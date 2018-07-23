cmake_minimum_required( VERSION 3.2 )
set( CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Supported configuration types" FORCE )

project( stlab LANGUAGES C CXX )

option( coverage "Enable binary instrumentation to collect test coverage information in the DEBUG configuration" )
option( stlab_testing "Compile the stlab tests and integrate with ctest" ${BUILD_TESTING} )
option( stlab_force_boost_optional "Force use of Boost optional." OFF)
option( stlab_force_boost_variant "Force use of Boost variant." OFF)
option( stlab_disable_future_coroutines "Disable use of coroutines." OFF)

set( Boost_MULTITHREADED ON ) 
set( Boost_USE_STATIC_LIBS ON )

if( EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake )
    include( ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake )
    set( CONAN_SYSTEM_INCLUDES ON )
    conan_basic_setup()
else()
    find_package( Boost 1.60.0 COMPONENTS unit_test_framework)
endif()

set( CMAKE_THREAD_PREFER_PTHREAD TRUE )
find_package( Threads )

add_library( stlab INTERFACE )

target_include_directories( stlab INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}"
                                  INTERFACE "${Boost_INCLUDE_DIRS}" )

target_link_libraries( stlab INTERFACE ${CMAKE_THREAD_LIBS_INIT}
                             INTERFACE ${CONAN_LIBS} )
                           
target_sources( stlab INTERFACE
                "${CMAKE_CURRENT_SOURCE_DIR}/stlab/concurrency/channel.hpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/stlab/concurrency/config.hpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/stlab/concurrency/default_executor.hpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/stlab/concurrency/executor_base.hpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/stlab/concurrency/future.hpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/stlab/concurrency/immediate_executor.hpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/stlab/concurrency/main_executor.hpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/stlab/concurrency/optional.hpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/stlab/concurrency/progress.hpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/stlab/concurrency/system_timer.hpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/stlab/concurrency/task.hpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/stlab/concurrency/traits.hpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/stlab/concurrency/tuple_algorithm.hpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/stlab/concurrency/utility.hpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/stlab/concurrency/variant.hpp")

if( ${stlab_disable_future_coroutines} )
  target_compile_definitions( stlab INTERFACE STLAB_DISABLE_FUTURE_COROUTINES )
endif()

if( ${Boost_FOUND} )
  if( ${stlab_force_boost_optional} )
    target_compile_definitions( stlab INTERFACE STLAB_FORCE_BOOST_OPTIONAL)
  endif()
  if( ${stlab_force_boost_variant} )
    target_compile_definitions( stlab INTERFACE STLAB_FORCE_BOOST_VARIANT)
  endif()
endif()

set( flags "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${CMAKE_CXX_COMPILER_ID}.cmake" )
if( EXISTS ${flags} )
  include( ${flags} )
else()
  message( WARNING "No stlab-defined flags for ${CMAKE_CXX_COMPILER_ID} C++ compiler")
  message( STATUS "Only CMake defaults will be used")
endif()
target_compile_options( stlab INTERFACE ${stlab_interface_flags} )
              
if ( stlab_testing )
  enable_testing()
  add_subdirectory( test )
endif()

