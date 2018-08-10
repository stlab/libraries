#
# If the current compiler doesn't support coroutines and coroutine support
# has been requested, issue a warning indicating the tests will exercise the
# coroutine integration in the stlab library.
#
# Note that 2017 is required due to the update to co_yield and co_await from
# yield and await keywords, respectively, used previously in MSVC.
#
if( CMAKE_CXX_COMPILER_ID STREQUAL "MSVC"
    AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 2017
    AND stlab.coroutines )
  message( WARNING "${CMAKE_CXX_COMPILER} does not support coroutines (as used by stlab)." )
  message( STATUS "Coroutines will not be used in testing" )
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
       ",$<NOT:$<VERSION_LESS:$<CXX_COMPILER_VERSION>,2017>>>" )

#
# If using MSVC and active, set the coroutines flag
#
string( CONCAT flag_generator
  "$<$<AND:$<CXX_COMPILER_ID:MSVC>,${active}>:/await>" )

target_compile_options( coroutines INTERFACE ${flag_generator} )

#
# If using MSVC and not active, set a preprocessor definition to disable
# the use of coroutines in the headers.
#
string( CONCAT definition_generator
  "$<$<AND:$<CXX_COMPILER_ID:MSVC>,$<NOT:${active}>>"
    ":STLAB_DISABLE_FUTURE_COROUTINES>" )

target_compile_definitions( coroutines INTERFACE ${definition_generator} )
