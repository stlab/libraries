cmake_minimum_required( VERSION 3.5 )

#
# Here we check whether stlab is being configured in isolation or as a component
# of a larger proeject. To do so, we query whether the `PROJECT_NAME` CMake
# variable has been defined. In the case it has, we can conclude stlab is a
# subproject.
#
# This convention has been borrowed from the Catch C++ unit testing library.
#
if( DEFINED PROJECT_NAME )
  set( subproject ON )
else()
  set( subproject OFF )
endif()

project( stlab VERSION 1.0 LANGUAGES CXX )

include( CTest )
include( CMakeDependentOption )

#
# The `stlab.testing` and `stlab.coverage` options only appear as
# cmake-gui and ccmake options iff stlab is the highest level project.
# In the case that stlab is a subproject, these options are hidden from
# the user interface and set to `OFF`
#
cmake_dependent_option( stlab.testing
  "Compile the stlab tests and integrate with ctest"
  ${BUILD_TESTING} "NOT subproject" OFF )

cmake_dependent_option( stlab.coverage
  "Enable binary instrumentation to collect test coverage information in the DEBUG configuration"
  OFF "NOT subproject" OFF )

option( stlab.boost_variant "Prefer Boost::variant to std::variant" OFF )
option( stlab.boost_optional "Prefer Boost::optional to std::optional" OFF )
option( stlab.coroutines "Leverage the coroutine TS in stlab" OFF )

#
# On apple we have to force the usage of boost.varianr, because Apple's
# implementation of C++17 is not complete
#
if(APPLE AND CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  set( stlab.boost_variant ON )
endif()

mark_as_advanced( stlab.coroutines stlab.boost_variant stlab.boost_optional )

#
# stlab has no compiled components. As such, we declare it as an `INTERFACE`
# library, which denotes a collection of target propeties to be applied
# transitively to linking targets. In our case, this ammounts to an include
# directory, compile flags, linking flags, and links to system libraries.
#
add_library( stlab INTERFACE )
add_library( stlab::stlab ALIAS stlab )

#
# stlab requires C++ 14 support, at a minimum. Setting the `cxx_std_14` compile
# features ensures that the corresponding C++ standard flag is populated in
# targets linking to stlab.
#
target_compile_features( stlab INTERFACE cxx_std_17 )

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
set( CMAKE_THREAD_PREFER_PTHREAD TRUE )
find_package( Threads REQUIRED )
target_link_libraries( stlab INTERFACE Threads::Threads )

#
# Several definitions are specified for the microsoft compiler. These have
# the following effects.
#
# + NOMINMAX
#    disable the `min` and `max` macros defined in the windows.h header
#
target_compile_definitions( stlab INTERFACE $<$<CXX_COMPILER_ID:MSVC>:NOMINMAX> )

add_subdirectory( stlab/concurrency )

if ( stlab.testing OR stlab.boost_variant OR stlab.boost_optional )
  if( EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include( ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake )

    #
    # using the `TARGETS` keyword in the `conan_basic_setup` invocation
    # will populate targets for each conan package specified in the
    # conanfile.txt
    #
    conan_basic_setup( TARGETS )

    #
    # We provide aliases to the conan target to mimic the native findBoost
    # functionality
    #
    add_library( Boost::boost INTERFACE IMPORTED )
    add_library( Boost::unit_test_framework INTERFACE IMPORTED )
    set_property( TARGET Boost::boost Boost::unit_test_framework 
      APPEND PROPERTY INTERFACE_LINK_LIBRARIES CONAN_PKG::boost )
  else()

    #
    # Request compiled unit testing component only if testing is `ON`
    #
    if( stlab.testing )
      find_package( Boost 1.60.0 REQUIRED COMPONENTS unit_test_framework )
    else()
      find_package( Boost 1.60.0 REQUIRED )
    endif()
  endif()


  if(NOT TARGET Boost::unit_test_framework)
    message(FATAL_ERROR "Could not find Boost unit test framework.")
  endif()
  

  string( APPEND either_generator
    "$<OR:$<BOOL:${stlab.boost_optional}>,"
         "$<BOOL:${stlab.boost_variant}>>" )

  #
  # Link to the `Boost::boost` target is either `stlab.boost_optional`
  # or `stlab.boost_variant` are set `ON`, which provides the include
  # directory for the boost header.
  #
  target_link_libraries( stlab INTERFACE $<${either_generator}:Boost::boost> )

  #
  # Conditionally specify the corresponding compiler definitions for
  # each boost algebraic data type library. In the case that either
  # is specified, a preprocessor definition is used to specify that
  # `auto_ptr` should not be used in the boost headers.
  #
  target_compile_definitions( stlab INTERFACE
    $<$<BOOL:${stlab.boost_optional}>:STLAB_FORCE_BOOST_OPTIONAL>
    $<$<BOOL:${stlab.boost_variant}>:STLAB_FORCE_BOOST_VARIANT> )

  unset( either_generator )
endif()

list( APPEND CMAKE_MODULE_PATH "${stlab_SOURCE_DIR}/cmake" )
include( stlab/coroutines )
target_link_libraries( stlab INTERFACE stlab::coroutines )

if ( stlab.testing )
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
install( TARGETS stlab coroutines EXPORT stlabTargets )

#
# Non-testing header files (preserving relative paths) are installed to the
# `include` subdirectory of the `$INSTALL_DIR/${CMAKE_INSTALL_PREFIX}`
# directory. Source file permissions preserved.
#
install( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/stlab
  DESTINATION include
  USE_SOURCE_PERMISSIONS
  FILES_MATCHING
    PATTERN "*.hpp"
    PATTERN "*test*" EXCLUDE )

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
set( CMAKE_PROJECT_HOMEPAGE_URL "http://stlab.cc/libraries/" )
include( CPack )
