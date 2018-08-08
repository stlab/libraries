cmake_minimum_required( VERSION 3.5 )

if( DEFINED PROJECT_NAME )
  set( root OFF )
  set( subproject ON )
else()
  set( root ON )
  set( subproject OFF )
endif()

project( stlab VERSION 1.0 LANGUAGES C CXX )

include( CTest )
include( CMakeDependentOption )
include( CMakePackageConfigHelpers )

cmake_dependent_option( stlab.testing
  "Compile the stlab tests and integrate with ctest"
  ON "root;BUILD_TESTING" OFF )

cmake_dependent_option( stlab.coverage
  "Enable binary instrumentation to collect test coverage information in the DEBUG configuration"
  OFF "root" OFF )

option( stlab.coroutines "Integrate the C++17 coroutines TS in stlab" OFF )
mark_as_advanced( stlab.coroutines )

add_library( stlab INTERFACE )
add_library( stlab::stlab ALIAS stlab )

target_compile_features( stlab INTERFACE cxx_std_14 )
target_include_directories( stlab INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include> )

set( CMAKE_THREAD_PREFER_PTHREAD TRUE )
find_package( Threads REQUIRED )
target_link_libraries( stlab INTERFACE Threads::Threads)

target_sources( stlab INTERFACE
  $<BUILD_INTERFACE:
      ${stlab_SOURCE_DIR}/stlab/concurrency/channel.hpp
      ${stlab_SOURCE_DIR}/stlab/concurrency/config.hpp
      ${stlab_SOURCE_DIR}/stlab/concurrency/default_executor.hpp
      ${stlab_SOURCE_DIR}/stlab/concurrency/executor_base.hpp
      ${stlab_SOURCE_DIR}/stlab/concurrency/future.hpp
      ${stlab_SOURCE_DIR}/stlab/concurrency/immediate_executor.hpp
      ${stlab_SOURCE_DIR}/stlab/concurrency/main_executor.hpp
      ${stlab_SOURCE_DIR}/stlab/concurrency/optional.hpp
      ${stlab_SOURCE_DIR}/stlab/concurrency/progress.hpp
      ${stlab_SOURCE_DIR}/stlab/concurrency/system_timer.hpp
      ${stlab_SOURCE_DIR}/stlab/concurrency/task.hpp
      ${stlab_SOURCE_DIR}/stlab/concurrency/traits.hpp
      ${stlab_SOURCE_DIR}/stlab/concurrency/tuple_algorithm.hpp
      ${stlab_SOURCE_DIR}/stlab/concurrency/utility.hpp
      ${stlab_SOURCE_DIR}/stlab/concurrency/variant.hpp>
  $<INSTALL_INTERFACE:
      include/stlab/concurrency/channel.hpp
      include/stlab/concurrency/config.hpp
      include/stlab/concurrency/default_executor.hpp
      include/stlab/concurrency/executor_base.hpp
      include/stlab/concurrency/future.hpp
      include/stlab/concurrency/immediate_executor.hpp
      include/stlab/concurrency/main_executor.hpp
      include/stlab/concurrency/optional.hpp
      include/stlab/concurrency/progress.hpp
      include/stlab/concurrency/system_timer.hpp
      include/stlab/concurrency/task.hpp
      include/stlab/concurrency/traits.hpp
      include/stlab/concurrency/tuple_algorithm.hpp
      include/stlab/concurrency/utility.hpp
      include/stlab/concurrency/variant.hpp> )

if ( stlab.testing )
  list( APPEND CMAKE_MODULE_DIR "${stlab_SOURCE_DIR}/cmake" )
#   include( development )

  if( EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake )
    include( conanbuildinfo.cmake )
    conan_basic_setup( TARGETS )
    add_library( Boost::unit_test_framework ALIAS CONAN_PKG::Boost )
  else()
    find_package( Boost 1.60.0 REQUIRED COMPONENTS unit_test_framework )
  endif()


  add_library( testing INTERFACE )
  target_link_libraries( testing INTERFACE
    Boost::unit_test_framework
#     stlab::development
    stlab::stlab )

  target_compile_definitions( testing INTERFACE
    $<$<NOT:$<BOOL:${BOOST_Boost_USE_STATIC_LIBS}>>:BOOST_TEST_DYN_LINK> )

  set_target_properties( testing PROPERTIES
    INTERFACE_COROUTINES ${stlab.coroutines}
    INTERFACE_CXX_EXTENSIONS OFF )

  set_property( TARGET testing APPEND PROPERTY
    COMPATIBLE_INTERFACE_BOOL INTERFACE_COROUTINES)

  add_library( stlab::testing ALIAS testing )

  add_subdirectory( test )
endif()

write_basic_package_version_file(
  "${stlab_BINARY_DIR}/stlabConfigVersion.cmake"
  VERSION ${stlab_VERSION}
  COMPATIBILITY SameMajorVersion )

install( TARGETS stlab
  EXPORT stlabTargets
  BUNDLE        DESTINATION bin     COMPONENT Runtime
  RUNTIME       DESTINATION bin     COMPONENT Runtime
  LIBRARY       DESTINATION lib     COMPONENT Runtime
  ARCHIVE       DESTINATION lib     COMPONENT Development
  PUBLIC_HEADER DESTINATION include COMPONENT Development )

install( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/stlab
  DESTINATION include
  USE_SOURCE_PERMISSIONS
  FILES_MATCHING PATTERN "*.hpp")

install( EXPORT stlabTargets
  FILE stlabTargets.cmake
  NAMESPACE stlab::
  DESTINATION share/cmake/stlab )

install( FILES
  "${stlab_SOURCE_DIR}/cmake/stlabConfig.cmake"
  "${stlab_BINARY_DIR}/stlabConfigVersion.cmake"
  DESTINATION share/cmake/stlab )

