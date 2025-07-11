cmake_minimum_required(VERSION 3.23)

project(stlab VERSION 2.0.1 LANGUAGES CXX)

# Create the main library target first
add_library(stlab)
add_library(stlab::stlab ALIAS stlab)

########################################################
# Dependencies

# download CPM.cmake
file(
    DOWNLOAD
    https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.40.8/CPM.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
    EXPECTED_HASH SHA256=78ba32abdf798bc616bab7c73aac32a17bbd7b06ad9e26a6add69de8f3ae4791
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

# Enable CPM caching to avoid re-downloading dependencies
set(CPM_SOURCE_CACHE ${CMAKE_SOURCE_DIR}/.cpm-cache CACHE PATH "Directory to cache CPM packages" FORCE)

# Add stlab-copy-on-write as a dependency
CPMAddPackage("gh:stlab/copy-on-write@1.0.3")
target_link_libraries(stlab INTERFACE stlab::copy-on-write)

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
if (STLAB_NO_STD_COROUTINES)
  set(STLAB_STD_COROUTINES 0)
else()
  set(STLAB_STD_COROUTINES 1)
endif()

stlab_detect_thread_system(STLAB_DEFAULT_THREAD_SYSTEM)
set(STLAB_THREAD_SYSTEM ${STLAB_DEFAULT_THREAD_SYSTEM} CACHE STRING "Thread system to use (win32|pthread|pthread-emscripten|pthread-apple|none)")

option(STLAB_MINIMAL_TASK_POOL "Use the minimal task pool size (ON|OFF), implies `STLAB_TASK_SYSTEM=portable`" OFF)
if(STLAB_MINIMAL_TASK_POOL)
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
# and installaion. Here we use a CMake generator expression to dispatch
# on how the configuration under which this library is being consumed.
#
target_include_directories(stlab INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
  $<INSTALL_INTERFACE:include>)

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

if(BUILD_TESTING)
  add_subdirectory(test)
endif()

include(CMakePackageConfigHelpers) # provides `write_basic_package_version_file`
write_basic_package_version_file(
  "${stlab_BINARY_DIR}/stlabConfigVersion.cmake"
  VERSION ${stlab_VERSION}
  COMPATIBILITY SameMajorVersion)

install(
  TARGETS stlab
  EXPORT stlabTargets
  FILE_SET stlab
)

#
# A CMake configuration file is generated describing the stlab exported targets.
# This file is included by (and installed with) the cmake/CMakeConfig.cmake file
# under version control.
#
install(EXPORT stlabTargets
  FILE stlabTargets.cmake
  NAMESPACE stlab::
  DESTINATION share/cmake/stlab)

#
# Install the CMake configuration files to the `share/cmake/stlab` subdirectory
# of `$INSTALL_DIR/${CMAKE_INSTALL_PREFIX}`. This path will be searched by
# default by the `find_package` intrinsic, provided
# `$INSTALL_DIR/${CMAKE_INSTALL_PREFIX}` is an element of the
# `CMAKE_PREFIX_PATH` environment variable.
#
configure_file(
  "${stlab_SOURCE_DIR}/cmake/stlabConfig.cmake.in"
  "${stlab_BINARY_DIR}/stlabConfig.cmake"
  @ONLY)
install(FILES
  "${stlab_BINARY_DIR}/stlabConfig.cmake"
  "${stlab_BINARY_DIR}/stlabConfigVersion.cmake"
  DESTINATION share/cmake/stlab)
