cmake_minimum_required( VERSION 3.23 )

project( stlab VERSION 1.6.2 LANGUAGES CXX )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/cmake)
include( CTest )
include( CMakeDependentOption )
include( StlabUtil )

find_package( Threads )
find_package( Boost 1.74.0 OPTIONAL_COMPONENTS unit_test_framework )
find_package( libdispatch )

cmake_dependent_option( stlab.coverage
  "Enable binary instrumentation to collect test coverage information in the DEBUG configuration"
  OFF PROJECT_IS_TOP_LEVEL OFF )

option( STLAB_USE_BOOST_CPP17_SHIMS "Use variant and optional from Boost instead of std. Useful for non-conforming compilers." OFF )
option( STLAB_NO_STD_COROUTINES "Suppress usage of standard coroutines. Useful for non-conforming compilers." FALSE )
stlab_detect_thread_system(STLAB_DEFAULT_THREAD_SYSTEM)
set( STLAB_THREAD_SYSTEM ${STLAB_DEFAULT_THREAD_SYSTEM} CACHE STRING "Thread system to use (win32|pthread|pthread-emscripten|pthread-apple|none)")

stlab_detect_task_system(STLAB_DEFAULT_TASK_SYSTEM)
set(STLAB_TASK_SYSTEM ${STLAB_DEFAULT_TASK_SYSTEM} CACHE STRING "Select the task system (portable|libdispatch|emscripten|windows).")

if( BUILD_TESTING AND NOT Boost_unit_test_framework_FOUND )
  message( SEND_ERROR "BUILD_TESTING is enabled, but an installation of Boost.Test was not found." )
endif()

if( STLAB_USE_BOOST_CPP17_SHIMS AND NOT Boost_FOUND )
  message( SEND_ERROR "STLAB_USE_BOOST_CPP17_SHIMS is enabled, but a Boost installation was not found." )
endif()

if( (NOT STLAB_THREAD_SYSTEM STREQUAL "none") AND NOT Threads_FOUND )
  message( SEND_ERROR "STLAB_THREAD_SYSTEM is not \"none\", but a thread system was not found." )
endif()

if( (STLAB_TASK_SYSTEM STREQUAL "libdispatch") AND NOT libdispatch_FOUND )
  message( SEND_ERROR "STLAB_TASK_SYSTEM is set to libdispatch, but a libdispatch installation was not found." )
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

if ( NOT STLAB_THREAD_SYSTEM STREQUAL "none" )
  target_link_libraries( stlab INTERFACE Threads::Threads )
endif()

#
# Several definitions are specified for the microsoft compiler. These have
# the following effects.
#
# + NOMINMAX
#    disable the `min` and `max` macros defined in the windows.h header
#
target_compile_definitions( stlab INTERFACE $<$<CXX_COMPILER_ID:MSVC>:NOMINMAX> )

add_subdirectory( stlab )

if ( STLAB_USE_BOOST_CPP17_SHIMS )
  target_link_libraries( stlab INTERFACE Boost::boost )
  target_compile_definitions( stlab INTERFACE STLAB_USE_BOOST_CPP17_SHIMS )
endif()

if ( STLAB_NO_STD_COROUTINES )
  target_compile_definitions( stlab INTERFACE STLAB_NO_STD_COROUTINES )
endif()

if ( STLAB_THREAD_SYSTEM STREQUAL "win32"  )
  target_compile_definitions( stlab INTERFACE STLAB_WIN32_THREADS )
elseif ( STLAB_THREAD_SYSTEM STREQUAL "pthread" )
  target_compile_definitions( stlab INTERFACE STLAB_PTHREAD_THREADS )
elseif ( STLAB_THREAD_SYSTEM STREQUAL "pthread-emscripten" )
  target_compile_definitions( stlab INTERFACE STLAB_PTHREAD_EMSCRIPTEN_THREADS )
elseif ( STLAB_THREAD_SYSTEM STREQUAL "pthread-apple" )
  target_compile_definitions( stlab INTERFACE STLAB_PTHREAD_APPLE_THREADS )
else ()
  target_compile_definitions( stlab INTERFACE STLAB_NO_THREADS )
endif()

if (${STLAB_TASK_SYSTEM} STREQUAL "portable")
  target_compile_definitions( stlab INTERFACE -DSTLAB_FORCE_TASK_SYSTEM_PORTABLE )
elseif (${STLAB_TASK_SYSTEM} STREQUAL "libdispatch")
  target_compile_definitions( stlab INTERFACE -DSTLAB_FORCE_TASK_SYSTEM_LIBDISPATCH )
  target_link_libraries(stlab INTERFACE libdispatch::libdispatch)
elseif (${STLAB_TASK_SYSTEM} STREQUAL "emscripten")
  target_compile_definitions( stlab INTERFACE -DSTLAB_FORCE_TASK_SYSTEM_EMSCRIPTEN )
elseif (${STLAB_TASK_SYSTEM} STREQUAL "windows")
  target_compile_definitions( stlab INTERFACE -DSTLAB_FORCE_TASK_SYSTEM_WINDOWS )
else()
  message(FATAL_ERROR "Invalid Task System: ${STLAB_TASK_SYSTEM}")
endif()

message(STATUS "stlab: Task System: ${STLAB_TASK_SYSTEM}")

list( APPEND CMAKE_MODULE_PATH "${stlab_SOURCE_DIR}/cmake" )

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
