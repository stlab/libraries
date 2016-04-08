cmake_minimum_required(VERSION 3.0)

project(SlabFuture C CXX)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")  
  set(CMAKE_CXX_FLAGS "-g -Wall -std=c++14 -fexceptions")
  #set(CMAKE_CXX_FLAGS "-O3 -g -Wall -std=c++14 -fexceptions")
  set(CMAKE_LINK_FLAGS "-pthreads")
endif()

if (APPLE OR WIN32)
    set(Boost_USE_STATIC_LIBS TRUE)
endif()

set (Boost_MULTITHREADED ON) 

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package( Threads )
if(CMAKE_USE_PTHREADS_INIT)
    set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-pthread")
endif()

find_package( Boost 1.60.0 COMPONENTS unit_test_framework system thread)

LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

include_directories(.)
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(src)
add_subdirectory(test)

