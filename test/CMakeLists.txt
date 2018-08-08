add_executable( stlab.test.channel.test
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

target_link_libraries( stlab.test.channel.test PUBLIC stlab::testing )
add_test( NAME stlab.test.channel COMMAND stlab.test.channel.test )

################################################################################

add_executable( stlab.test.future.test
                future_coroutine_tests.cpp
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

target_link_libraries( stlab.test.future.test PUBLIC stlab::testing )
add_test( NAME stlab.test.future COMMAND stlab.test.future.test )

################################################################################

add_executable( stlab.test.serial_queue.test
                serial_queue_test.cpp )

target_link_libraries( stlab.test.serial_queue.test PUBLIC stlab::testing )
add_test( NAME stlab.test.serial_queue COMMAND stlab.test.serial_queue.test )

################################################################################

add_executable( stlab.test.cow.test
                cow_test.cpp
                main.cpp )

target_link_libraries( stlab.test.cow.test PUBLIC stlab::testing )
add_test( NAME stlab.test.cow COMMAND stlab.test.cow.test )

################################################################################

add_executable( stlab.test.task.test
                task_test.cpp
                main.cpp )

target_link_libraries( stlab.test.task.test PUBLIC stlab::testing )
add_test( NAME stlab.test.task COMMAND stlab.test.task.test )

################################################################################

add_executable( stlab.test.tuple.test
                tuple_test.cpp
                main.cpp )

target_link_libraries( stlab.test.tuple.test PUBLIC stlab::testing )
add_test( NAME stlab.test.tuple COMMAND stlab.test.tuple.test )

