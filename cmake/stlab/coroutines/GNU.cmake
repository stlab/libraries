#
# If the current compiler doesn't support coroutines and coroutine support
# has been requested, issue a warning indicating the tests will not exercise
# the coroutine integration in the stlab library.
#
# As of writing, no version of the GNU C++ compiler supports the coroutines TS.
#
if( CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND stlab.coroutines )
  message( WARNING "${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION} does not support coroutines." )
  message( STATUS "Coroutines will not be used in testing" )
endif()
