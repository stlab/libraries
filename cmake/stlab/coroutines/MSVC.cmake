#
# If the current compiler doesn't support coroutines and coroutine support
# has been requested, issue a warning indicating the tests will not exercise
# the coroutine integration in the stlab library.
#
# TODO: Version 15.0.0 may be overly restrictive here. The minimum viable MSVC compiler
# version should be explored at some point in the future. 
#
if( CMAKE_CXX_COMPILER_ID STREQUAL "MSVC"
    AND stlab.coroutines )
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 15.0.0)
    message( WARNING "${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION} does not support coroutines (as used by stlab)." )
    message( STATUS "Coroutines will not be used in testing" )
  elseif(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 99.99.99)
    # TODO: replace version number
    # 99.99.99 is a place holder for a future microsoft compiler version that resolves
    # the await/co_await issue in the channels header observed in current MSVC compilers.
    message( WARNING "${CMAKE_CXX_COMPILER}-${CMAKE_CXX_COMPILER_VERSION} cannot accomodate the stlab/concurrency/channels header when compiling with coroutines enabled." )
    message( "This header will be excluded from the project definition." )
    message( "The corresponding tests will not be compiled." )
  endif()
endif()

#
# If the `COROUTINE` property has been established on the target and set
# to `ON` and the MSVC compiler version is sufficient to support the
# coroutine TS, return 1.
#
# Otherwise return 0.
#
string( CONCAT activate
  "$<AND:$<BOOL:$<TARGET_PROPERTY:COROUTINES>>"
       ",$<NOT:$<VERSION_LESS:$<CXX_COMPILER_VERSION>,15.0.0>"
         ">"
   ">" )

#
# If using MSVC and active, set the coroutines flag
#
target_compile_options( coroutines INTERFACE
  $<$<AND:$<CXX_COMPILER_ID:MSVC>,${active}>:/await> )

#
# If using MSVC and active, set a preprocessor definition to enable
# the use of coroutines in the headers and disable C++17 depreciation warnings.
#
string( CONCAT definition_generator
  "$<$<AND:$<CXX_COMPILER_ID:MSVC>,${active}>"
    ":_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS;"
     "STLAB_FUTURE_COROUTINES=1;"
   ">" )

target_compile_definitions( coroutines INTERFACE ${definition_generator} )
