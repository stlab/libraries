if( NOT ( ${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC"
      AND ${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS "99.99.99"
      AND stlab.coroutines ) )
  add_executable( stlab.test.channel
    channel_functor_tests.cpp
    channel_merge_round_robin_tests.cpp
    channel_merge_unordered_tests.cpp
    channel_merge_zip_with_tests.cpp
    channel_process_tests.cpp
    channel_test_helper.cpp
    channel_tests.cpp
    tuple_algorithm_test.cpp
    main.cpp
    channel_test_helper.hpp )
endif()

target_link_libraries( stlab.test.channel PUBLIC stlab::testing )
add_test( NAME stlab.test.channel COMMAND stlab.test.channel )

################################################################################

add_executable( stlab.test.executor
        executor_test.cpp
        main.cpp)

target_link_libraries( stlab.test.executor PUBLIC stlab::testing )
add_test( NAME stlab.test.executor COMMAND stlab.test.executor )


################################################################################

add_executable( stlab.test.future
  future_recover_tests.cpp
  future_test_helper.cpp
  future_tests.cpp
  future_then_tests.cpp
  future_when_all_arguments_tests.cpp
  future_when_all_range_tests.cpp
  future_when_any_arguments_tests.cpp
  future_when_any_range_tests.cpp
  tuple_algorithm_test.cpp
  main.cpp
  future_test_helper.hpp )

target_sources( stlab.test.future PUBLIC
  $<$<BOOL:$<TARGET_PROPERTY:COROUTINES>>:future_coroutine_tests.cpp> )
target_link_libraries( stlab.test.future PUBLIC stlab::testing )
add_test( NAME stlab.test.future COMMAND stlab.test.future )

################################################################################

add_executable( stlab.test.serial_queue
  serial_queue_test.cpp )

target_link_libraries( stlab.test.serial_queue PUBLIC stlab::testing )
add_test( NAME stlab.test.serial_queue COMMAND stlab.test.serial_queue )

################################################################################

add_executable( stlab.test.cow
  cow_test.cpp
  main.cpp )

target_link_libraries( stlab.test.cow PUBLIC stlab::testing )
add_test( NAME stlab.test.cow COMMAND stlab.test.cow )

################################################################################

add_executable( stlab.test.task
  task_test.cpp
  main.cpp )

target_link_libraries( stlab.test.task PUBLIC stlab::testing )
add_test( NAME stlab.test.task COMMAND stlab.test.task )

################################################################################

add_executable( stlab.test.tuple
  tuple_test.cpp
  main.cpp )

target_link_libraries( stlab.test.tuple PUBLIC stlab::testing )
add_test( NAME stlab.test.tuple COMMAND stlab.test.tuple )

################################################################################

add_executable( stlab.test.traits
        traits_test.cpp
        main.cpp )

target_link_libraries( stlab.test.traits PUBLIC stlab::testing )
add_test( NAME stlab.test.traits COMMAND stlab.test.traits )

################################################################################
#
# tests are compiled without compiler extensions to ensure the stlab headers
# are not dependent upon any such extension.
#
set_target_properties(
  stlab.test.channel
  stlab.test.future
  stlab.test.serial_queue
  stlab.test.cow
  stlab.test.task
  stlab.test.tuple
  stlab.test.traits
  PROPERTIES CXX_EXTENSIONS OFF )

#
# Many of the stlab tests are executed using the system executor which defaults
# to a number of threads equal to the system parallelism.
#
# Here we (attempt to) query the system processor count and store the value in
# the `nProcessors` variable, where a non-zeros value indicates success.
#
# Provided the query was successful, we inform ctest of the test parallism.
#

include(ProcessorCount)
ProcessorCount(nProcessors)

if(nProcessors)
  set_tests_properties(
    stlab.test.channel
    stlab.test.executor
    stlab.test.future
    stlab.test.serial_queue
    stlab.test.cow
    stlab.test.task
    stlab.test.tuple
    PROPERTIES PROCESSORS ${nProcessors})
endif()
