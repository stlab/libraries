cmake_minimum_required(VERSION 3.0)

project(SlabFuture C CXX)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")  
  set(CMAKE_CXX_FLAGS "-O3 -g -Wall -std=c++14")
  set(CMAKE_LINK_FLAGS "-pthreads")
endif()

set(BOOST_ROOT D:/boost_1_60_0)
set (Boost_USE_STATIC_LIBS OFF) 
set (Boost_MULTITHREADED ON) 

find_package( Boost 1.55.0 COMPONENTS unit_test_framework system thread)
find_package( Threads )

include_directories(.)
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(src)
add_subdirectory(test)

