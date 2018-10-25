#
# If the current compiler doesn't support coroutines and coroutine support
# has been requested, issue a warning indicating the tests will not exercise
# the coroutine integration in the stlab library.
#
if( CMAKE_CXX_COMPILER_ID STREQUAL "Clang"
    AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0
    AND stlab.coroutines )
  message( WARNING "${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION} does not support coroutines." )
  message( STATUS "Coroutines will not be used in testing" )
endif()

#
# If the `COROUTINE` property has been established on the target and set
# to `ON` and the Clang compiler version is sufficient to support the
# coroutine TS, return 1.
#
# Otherwise return 0.
#
string( CONCAT active
  "$<AND:$<BOOL:$<TARGET_PROPERTY:COROUTINES>>"
       ",$<NOT:$<VERSION_LESS:$<CXX_COMPILER_VERSION>,5>>>" )

#
# If using Clang and active, set the coroutines flag
#
string( CONCAT flag_generator
  "$<$<AND:$<CXX_COMPILER_ID:Clang>,${active}>:-fcoroutines-ts>" )

target_compile_options( coroutines INTERFACE ${flag_generator} )

#
# If using Clang and not active, set a preprocessor definition to disable
# the use of coroutines in the headers.
#
string( CONCAT definition_generator
  "$<$<AND:$<CXX_COMPILER_ID:Clang>,$<NOT:${active}>>"
    ":STLAB_FUTURE_COROUTINES=1>" )

target_compile_definitions( coroutines INTERFACE ${definition_generator} )
