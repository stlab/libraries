cmake_minimum_required(VERSION 3.0)

project(SlabFuture)

if(WIN32)
  add_definitions(-D_WIN32_WINNT=0x0501)
ENDIF(WIN32)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")  
  #set(CMAKE_CXX_FLAGS "-O3 -fsanitize=thread -g -Wall -std=c++11")
  set(CMAKE_CXX_FLAGS "-O3 -g -Wall -std=c++14")
  set(CMAKE_LINK_FLAGS "-pthreads")
endif()

set(BOOST_ROOT D:/boost_1_59_0)
set(Boost_USE_STATIC_LIBS        OFF)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME     OFF)
#set(BOOST_ALL_DYN_LINK           ON) 
find_package( Boost 1.55.0 REQUIRED COMPONENTS thread)


add_subdirectory(src)
add_subdirectory(test)

