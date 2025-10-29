cmake_minimum_required(VERSION 3.23)

project(stlab VERSION 2.1.4 LANGUAGES CXX)

# Create the main library target first
add_library(stlab)
add_library(stlab::stlab ALIAS stlab)

########################################################
# Dependencies

# Option to install stlab and its dependencies
option(STLAB_INSTALL "Enable installation of stlab and its dependencies" OFF)

# Enable CPM caching to avoid re-downloading dependencies (only when top level)
if(PROJECT_IS_TOP_LEVEL)
  set(CPM_SOURCE_CACHE ${CMAKE_SOURCE_DIR}/.cache/cpm CACHE PATH "Directory to cache CPM packages")
endif()
include(cmake/CPM.cmake)

# https://github.com/stlab/copy-on-write
CPMAddPackage("gh:stlab/copy-on-write@1.0.6")
target_link_libraries(stlab INTERFACE stlab::copy-on-write)

# https://github.com/stlab/enum-ops
CPMAddPackage("gh:stlab/enum-ops@1.1.0")
target_link_libraries(stlab INTERFACE stlab::enum-ops)

########################################################
# clangd

if(CMAKE_EXPORT_COMPILE_COMMANDS AND PROJECT_IS_TOP_LEVEL)
  add_custom_target(clangd_compile_commands ALL
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_BINARY_DIR}/compile_commands.json
    ${CMAKE_SOURCE_DIR}/compile_commands.json
    COMMENT "Creating symlink to compile_commands.json for clangd"
  )
endif()

########################################################

# Set the default C++ language version
set(CMAKE_CXX_STANDARD 20 CACHE STRING "The C++ standard to use")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set the default build type to Release
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(CMakeDependentOption)
include(StlabUtil)
# CTest must be included at the top level for tests to be found.
include(CTest)

find_package(libdispatch)
find_package(Qt5 QUIET COMPONENTS Core)
find_package(Qt6 QUIET COMPONENTS Core)
find_package(Threads)

cmake_dependent_option(stlab.coverage
  "Enable binary instrumentation to collect test coverage information in the DEBUG configuration"
  OFF PROJECT_IS_TOP_LEVEL OFF)

stlab_check_disfunctional_coroutines(STLAB_DEFAULT_NO_STD_COROUTINES)
option(STLAB_NO_STD_COROUTINES "Suppress usage of standard coroutines. Useful for non-conforming compilers." ${STLAB_DEFAULT_NO_STD_COROUTINES})
if(STLAB_NO_STD_COROUTINES)
  set(STLAB_STD_COROUTINES 0)
else()
  set(STLAB_STD_COROUTINES 1)
endif()

stlab_detect_thread_system(STLAB_DEFAULT_THREAD_SYSTEM)
set(STLAB_THREAD_SYSTEM ${STLAB_DEFAULT_THREAD_SYSTEM} CACHE STRING "Thread system to use (win32|pthread|pthread-emscripten|pthread-apple|none)")

set(STLAB_TASK_POOL_MAXIMUM 0 CACHE STRING "Define the maximum number threads in the task pool. Default of zero implies a pool size of std::thread::hardware_concurrency. Non-zero implies STLAB_TASK_SYSTEM=portable.")

# Ensure STLAB_TASK_POOL_MAXIMUM is a non-negative number
if(NOT STLAB_TASK_POOL_MAXIMUM MATCHES "^[0-9]+$")
  message(FATAL_ERROR "STLAB_TASK_POOL_MAXIMUM must be a non-negative, decimal integer.")
endif()

#
# TODO (bmedina): When STLab v3.0 is released, remove the deprecated STLAB_MINIMAL_TASK_POOL option
# and eliminate the code below.
#
option(STLAB_MINIMAL_TASK_POOL "Deprecated: Use STLAB_TASK_POOL_MAXIMUM=1." OFF)
if(STLAB_MINIMAL_TASK_POOL)
  message(WARNING "STLAB_MINIMAL_TASK_POOL is deprecated. Use STLAB_TASK_POOL_MAXIMUM=1, instead.")

  # The deprecated STLAB_MINIMAL_TASK_POOL implied a pool size of 1.
  # If STLAB_TASK_POOL_MAXIMUM is at its default of zero, make it 1.
  # Otherwise, if it's been customized to something other than 1, that's logically invalid.
  # Fail and direct them to the new option.
  if(STLAB_TASK_POOL_MAXIMUM EQUAL 0)
    set(STLAB_TASK_POOL_MAXIMUM 1)
  elseif(STLAB_TASK_POOL_MAXIMUM GREATER 1)
    message(FATAL_ERROR "STLAB_MINIMAL_TASK_POOL is deprecated and incompatible with STLAB_TASK_POOL_MAXIMUM>1. Use STLAB_TASK_POOL_MAXIMUM to set the desired pool size.")
  endif()
endif()

if(STLAB_TASK_POOL_MAXIMUM GREATER 0)
  set(STLAB_TASK_SYSTEM "portable" CACHE STRING "Task system to use (portable|libdispatch|windows)." FORCE)
else()
  stlab_detect_task_system(STLAB_DEFAULT_TASK_SYSTEM)
endif()
set(STLAB_TASK_SYSTEM ${STLAB_DEFAULT_TASK_SYSTEM} CACHE STRING "Task system to use (portable|libdispatch|windows).")

stlab_detect_main_executor(STLAB_DEFAULT_MAIN_EXECUTOR)
set(STLAB_MAIN_EXECUTOR ${STLAB_DEFAULT_MAIN_EXECUTOR} CACHE STRING "Main executor to use (qt5|qt6|libdispatch|emscripten|none).")

if((NOT STLAB_THREAD_SYSTEM STREQUAL "none") AND NOT Threads_FOUND)
  message(SEND_ERROR "STLAB_THREAD_SYSTEM is not \"none\", but a thread system was not found.")
endif()

if((STLAB_TASK_SYSTEM STREQUAL "libdispatch") AND NOT libdispatch_FOUND)
  message(SEND_ERROR "STLAB_TASK_SYSTEM is set to \"libdispatch\", but a libdispatch installation was not found.")
endif()

if((STLAB_MAIN_EXECUTOR STREQUAL "libdispatch") AND NOT libdispatch_FOUND)
  message(SEND_ERROR "STLAB_MAIN_EXECUTOR is set to \"libdispatch\", but a libdispatch installation was not found.")
endif()

if((STLAB_MAIN_EXECUTOR STREQUAL "qt5") AND NOT Qt5Core_FOUND)
  message(SEND_ERROR "STLAB_MAIN_EXECUTOR is set to \"qt5\", but a Qt5 installation was not found.")
endif()

if((STLAB_MAIN_EXECUTOR STREQUAL "qt6") AND NOT Qt6Core_FOUND)
  message(SEND_ERROR "STLAB_MAIN_EXECUTOR is set to \"qt6\", but a Qt6 installation was not found.")
endif()

#
# The include directory for stlab can be expected to vary between build
# and installation. Here we use a CMake generator expression to dispatch
# on how the configuration under which this library is being consumed.
#
target_include_directories(stlab INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

#
# Several definitions are specified for the microsoft compiler. These have
# the following effects.
#
# + NOMINMAX
#    disable the `min` and `max` macros defined in the windows.h header
#
target_compile_definitions(stlab INTERFACE $<$<CXX_COMPILER_ID:MSVC>:NOMINMAX>)

add_subdirectory(include/stlab)
add_subdirectory(src)

if(NOT STLAB_THREAD_SYSTEM STREQUAL "none")
  target_link_libraries(stlab INTERFACE Threads::Threads)
endif()

if(STLAB_TASK_SYSTEM STREQUAL "libdispatch")
  target_link_libraries(stlab INTERFACE libdispatch::libdispatch)
endif()

if(STLAB_MAIN_EXECUTOR STREQUAL "libdispatch")
  target_link_libraries(stlab INTERFACE libdispatch::libdispatch)
elseif(STLAB_MAIN_EXECUTOR STREQUAL "qt5")
  target_link_libraries(stlab INTERFACE Qt5::Core)
elseif(STLAB_MAIN_EXECUTOR STREQUAL "qt6")
  target_link_libraries(stlab INTERFACE Qt6::Core)
endif()

message(STATUS "stlab: Disable Coroutines: ${STLAB_DEFAULT_NO_STD_COROUTINES}")
message(STATUS "stlab: Thread System: ${STLAB_THREAD_SYSTEM}")
message(STATUS "stlab: Task System: ${STLAB_TASK_SYSTEM}")
message(STATUS "stlab: Main Executor: ${STLAB_MAIN_EXECUTOR}")

if(PROJECT_IS_TOP_LEVEL AND BUILD_TESTING)
  add_subdirectory(test)
endif()

########################################################
# Installation

if(STLAB_INSTALL)
  include(GNUInstallDirs)
  include(CMakePackageConfigHelpers)

  # Install copy-on-write if it was added by CPM
  # Should be moved to copy-on-write or cpp_library
  if(copy-on-write_ADDED)
    set_target_properties(stlab-copy-on-write PROPERTIES
      EXPORT_NAME copy-on-write
    )
    install(TARGETS stlab-copy-on-write
      EXPORT stlab-targets
      FILE_SET headers DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
  endif()

  # Install enum-ops if it was added by CPM
  # Should be moved to copy-on-write or cpp_library
  if(enum-ops_ADDED)
    set_target_properties(stlab-enum-ops PROPERTIES
      EXPORT_NAME enum-ops
    )
    install(TARGETS stlab-enum-ops
      EXPORT stlab-targets
      FILE_SET headers DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
  endif()

  install(TARGETS stlab
    EXPORT stlab-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    FILE_SET stlab DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

  set(CONFIG_INSTALL_DIR cmake)

  install(EXPORT stlab-targets
    FILE stlab-targets.cmake
    NAMESPACE stlab::
    DESTINATION ${CONFIG_INSTALL_DIR}
  )

  configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/stlab-config.tmpl.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/stlab-config.cmake"
    INSTALL_DESTINATION ${CONFIG_INSTALL_DIR}
  )

  write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/stlab-config-version.cmake"
    VERSION ${stlab_VERSION}
    COMPATIBILITY SameMajorVersion
  )

  install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/stlab-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/stlab-config-version.cmake"
    DESTINATION ${CONFIG_INSTALL_DIR}
  )
endif()
