cmake_minimum_required( VERSION 3.23 )

project( stlab VERSION 1.6.2 LANGUAGES CXX )

include( CTest )
include( CMakeDependentOption )

find_package( Threads REQUIRED )
find_package( Boost 1.60.0 OPTIONAL_COMPONENTS unit_test_framework )

cmake_dependent_option( stlab.coverage
  "Enable binary instrumentation to collect test coverage information in the DEBUG configuration"
  OFF PROJECT_IS_TOP_LEVEL OFF )

option( stlab.boost_variant "Prefer Boost::variant to std::variant" OFF )
option( stlab.boost_optional "Prefer Boost::optional to std::optional" OFF )
option( stlab.coroutines "Leverage the coroutine TS in stlab" OFF )

set(stlab.task_system "header" CACHE STRING "Select the task system (header|portable|libdispatch|emscripten|pnacl|windows).")

#
# On apple we have to force the usage of boost.variant, because Apple's
# implementation of C++17 is not complete
#
if(APPLE AND (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang") AND CMAKE_CXX_COMPILER_VERSION LESS 12)
  message(STATUS "Apple-Clang versions less than 12 do not correctly support std::optional or std::variant, so we will use boost::optional and boost::variant instead.")
  set( stlab.boost_variant ON )
endif()

mark_as_advanced( stlab.coroutines stlab.boost_variant stlab.boost_optional )


if( BUILD_TESTING AND NOT Boost_unit_test_framework_FOUND )
  message( SEND_ERROR "BUILD_TESTING is enabled, but an installation of Boost.Test was not found." )
endif()

if( ( stlab.boost_variant OR stlab.boost_optional ) AND NOT Boost_FOUND )
  message( SEND_ERROR "stlab.boost_variant OR stlab.boost_optional is enabled, but a Boost installation was not found." )
endif()

#
# stlab has no compiled components. As such, we declare it as an `INTERFACE`
# library, which denotes a collection of target propeties to be applied
# transitively to linking targets. In our case, this ammounts to an include
# directory, compile flags, linking flags, and links to system libraries.
#
add_library( stlab INTERFACE )
add_library( stlab::stlab ALIAS stlab )

#
# The include directory for stlab can be expected to vary between build
# and installaion. Here we use a CMake generator expression to dispatch
# on how the configuration under which this library is being consumed.
#
target_include_directories( stlab INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include> )

#
# As of CMake version 3.1, the FindThreads CMake module supplies an imported
# target called `Thread::Thread` which transitively supplies inlude directories,
# compiler flags, and linker flags to CMake targets linking to it.
#
target_link_libraries( stlab INTERFACE Threads::Threads )

#
# Several definitions are specified for the microsoft compiler. These have
# the following effects.
#
# + NOMINMAX
#    disable the `min` and `max` macros defined in the windows.h header
#
target_compile_definitions( stlab INTERFACE $<$<CXX_COMPILER_ID:MSVC>:NOMINMAX> )

add_subdirectory( stlab )

if ( stlab.boost_variant OR stlab.boost_optional )
  target_link_libraries( stlab INTERFACE Boost::boost )

  target_compile_definitions( stlab INTERFACE
    $<$<BOOL:${stlab.boost_optional}>:STLAB_FORCE_BOOST_OPTIONAL>
    $<$<BOOL:${stlab.boost_variant}>:STLAB_FORCE_BOOST_VARIANT> )
endif()

if (NOT APPLE AND (${stlab.task_system} STREQUAL "libdispatch"))
  message(STATUS "CMAKE_PROJECT_NAME: ${CMAKE_PROJECT_NAME}")

  add_library( libdispatch::libdispatch INTERFACE IMPORTED )
  set_property( TARGET libdispatch::libdispatch
    APPEND PROPERTY INTERFACE_LINK_LIBRARIES CONAN_PKG::libdispatch )

  target_link_libraries(${CMAKE_PROJECT_NAME} INTERFACE libdispatch::libdispatch)
endif()

if (${stlab.task_system} STREQUAL "header")
  # Task System is selected in concurrency/config_task_system.hpp
  # Nothing to define.
elseif (${stlab.task_system} STREQUAL "portable")
  target_compile_definitions( stlab INTERFACE -DSTLAB_FORCE_TASK_SYSTEM_PORTABLE )
elseif (${stlab.task_system} STREQUAL "libdispatch")
  target_compile_definitions( stlab INTERFACE -DSTLAB_FORCE_TASK_SYSTEM_LIBDISPATCH )
elseif (${stlab.task_system} STREQUAL "emscripten")
  target_compile_definitions( stlab INTERFACE -DSTLAB_FORCE_TASK_SYSTEM_EMSCRIPTEN )
elseif (${stlab.task_system} STREQUAL "pnacl")
  target_compile_definitions( stlab INTERFACE -DSTLAB_FORCE_TASK_SYSTEM_PNACL )
elseif (${stlab.task_system} STREQUAL "windows")
  target_compile_definitions( stlab INTERFACE -DSTLAB_FORCE_TASK_SYSTEM_WINDOWS )
else()
  message(FATAL_ERROR "Invalid Task System: ${stlab.task_system}")
endif()

message(STATUS "stlab: Task System: ${stlab.task_system}")

list( APPEND CMAKE_MODULE_PATH "${stlab_SOURCE_DIR}/cmake" )

if( stlab.coroutines )
  include( stlab/coroutines )
  target_link_libraries( stlab INTERFACE stlab::coroutines )
endif()

if ( BUILD_TESTING )
  include( stlab/development )

  #
  # Establish a convenience target to encapsulate the properties common to the
  # stlab tests and establish an alias for uniformity.
  #
  add_library( testing INTERFACE )
  add_library( stlab::testing ALIAS testing )

  #
  # CMake targets linking to the stlab::testing target will (transitively)
  # link to the Boost::unit_test_framework and to stlab::stlab target.
  #
  target_link_libraries( testing INTERFACE
    Boost::unit_test_framework
    stlab::development
    stlab::stlab )

  #
  # Linking to the Boost unit test framework requires an additional
  # preprocessor definition when the unit test compiled resources are
  # provided by a shared library rather than a static library.
  #
  target_compile_definitions( testing INTERFACE
    $<$<NOT:$<BOOL:${Boost_USE_STATIC_LIBS}>>:BOOST_TEST_DYN_LINK>)

  add_subdirectory( test )
endif()

include( CMakePackageConfigHelpers ) # provides `write_basic_package_version_file`

#
# We generate a CMake version file for later installation to be consumed by
# CMake's `find_package` intrinsic. Here we specify a semantic version
# convention, i.e., backwards compatability can be assumed within a Major
# version.
#
write_basic_package_version_file(
  "${stlab_BINARY_DIR}/stlabConfigVersion.cmake"
  VERSION ${stlab_VERSION}
  COMPATIBILITY SameMajorVersion )

#
# As a header-only library, there are no target components to be installed
# directly (the PUBLIC_HEADER property is not white listed for INTERFACE
# targets for some reason).
#
# However, it is worthwhile export our target description in order to later
# generate a CMake configuration file for consumption by CMake's `find_package`
# intrinsic
#

if( stlab.coroutines )
    install(
      TARGETS stlab coroutines
      EXPORT stlabTargets
      FILE_SET stlab
    )
else()
    install(
      TARGETS stlab
      EXPORT stlabTargets
      FILE_SET stlab
    )
endif()

#
# A CMake configuration file is generated describing the stlab exported targets.
# This file is included by (and installed with) the cmake/CMakeConfig.cmake file
# under version control.
#
install( EXPORT stlabTargets
  FILE stlabTargets.cmake
  NAMESPACE stlab::
  DESTINATION share/cmake/stlab )

#
# Install the CMake configuration files to the `share/cmake/stlab` subdirectory
# of `$INSTALL_DIR/${CMAKE_INSTALL_PREFIX}`. This path will be searched by
# default by the `find_package` intrinsic, provided
# `$INSTALL_DIR/${CMAKE_INSTALL_PREFIX}` is an element of the
# `CMAKE_PREFIX_PATH` environment variable.
#
install( FILES
  "${stlab_SOURCE_DIR}/cmake/stlabConfig.cmake"
  "${stlab_BINARY_DIR}/stlabConfigVersion.cmake"
  DESTINATION share/cmake/stlab )

#
# Rudimentary CPack support.
#
# CPack provides a mechanism to generate installation packaging for a project,
# e.g., self-extracting shell scripts, compressed tarballs, Debian Package files,
# RPM Package Manager files, Windows NSIS installation wizards,
# Apple Disk Images (.dmg), etc.
#
# Any system libraries required (runtimes, threading, etc) should be bundled
# with the project for this type of installation. The
# `InstallRequiredSystemLibraries` CMake module attempts to provide this
# functionality in an automated way. Additional libraries may be specified as
#
# ```cmake
# list(APPEND CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS <library>)
# ```
#
# A packaged installation can be generated by calling
#
# ```sh
# cpack -G <packaging type> --config CPackConfig.cmake
# ```
#
# See `cpack --help` or the CPack documentation for more information.
#
include( InstallRequiredSystemLibraries )
set( CPACK_PACKAGE_VENDOR "Adobe Software Technology Lab" )
set( CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE" )
set( CMAKE_PROJECT_HOMEPAGE_URL "https://stlab.cc/libraries/" )
include( CPack )
