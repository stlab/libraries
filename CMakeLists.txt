cmake_minimum_required( VERSION 3.3 )
set( CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Supported configuration types" FORCE )

project( stlab VERSION 1.0.0 LANGUAGES C CXX )

option( coverage "Enable binary instrumentation to collect test coverage information in the DEBUG configuration" )
option( stlab_testing "Compile the stlab tests and integrate with ctest" ${BUILD_TESTING} )

add_library( stlab INTERFACE )
target_include_directories(stlab INTERFACE
        $<BUILD_INTERFACE:${stlab_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)
set( CMAKE_THREAD_PREFER_PTHREAD TRUE )
find_package( Threads )
target_link_libraries( stlab INTERFACE Threads::Threads ${CONAN_LIBS} )

if( EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake )
    include( ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake )
    set( CONAN_SYSTEM_INCLUDES ON )
    conan_basic_setup()
else()
    set( Boost_MULTITHREADED ON ) 
    set( Boost_USE_STATIC_LIBS ON )
    find_package( Boost 1.60.0 COMPONENTS unit_test_framework)
    target_link_libraries( stlab INTERFACE Boost::unit_test_framework)
endif()

set( flags "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${CMAKE_CXX_COMPILER_ID}.cmake" )
if( EXISTS ${flags} )
  include( ${flags} )
else()
  message( WARNING "No stlab-defined flags for ${CMAKE_CXX_COMPILER_ID} C++ compiler")
  message( STATUS "Only CMake defaults will be used")
endif()
target_compile_options( stlab INTERFACE ${stlab_interface_flags} )
              
# Setup install
install(TARGETS stlab EXPORT stlabTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include)
install(EXPORT stlabTargets
        FILE stlabTargets.cmake
        NAMESPACE stlab::
        DESTINATION lib/cmake/stlab)

include(CMakePackageConfigHelpers)
write_basic_package_version_file("stlabConfigVersion.cmake"
        VERSION ${stlab_VERSION}
        COMPATIBILITY SameMajorVersion)
install(FILES config/stlabConfig.cmake 
              ${stlab_BINARY_DIR}/stlabConfigVersion.cmake
        DESTINATION lib/cmake/stlab)
install(DIRECTORY include/stlab/concurrency DESTINATION include FILES_MATCHING
        PATTERN "*.hpp")


if ( stlab_testing )
  enable_testing()
  add_subdirectory( test )
endif()

