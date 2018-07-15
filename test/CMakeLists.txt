####################################################################################################

add_executable( stlab.test.channel.test
        "${CMAKE_CURRENT_SOURCE_DIR}/channel_functor_tests.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/channel_merge_round_robin_tests.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/channel_merge_unordered_tests.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/channel_merge_zip_with_tests.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/channel_process_tests.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/channel_test_helper.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/channel_tests.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/tuple_algorithm_test.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/channel_test_helper.hpp" )

# This syntax is a CMake generator expression.
#
# Here we defer the compiler flags decision-making to generation time
# (for tools which support distinct configuration and generation steps)
# based on a generation time configuration.
#
# Here, we provide support for a DEBUG and RELEASE configurations, line 32
# providing the former and line 33 providing the latter.
target_compile_options( stlab.test.channel.test
        PUBLIC ${stlab_base_flags}
        $<$<CONFIG:DEBUG>:$<$<BOOL:${coverage}>:${stlab_coverage_flags}>${stlab_debug_flags}>
        $<$<CONFIG:RELEASE>:${stlab_release_flags}>
        ${stlab_appended_flags} )

target_link_libraries( stlab.test.channel.test
        PUBLIC ${Boost_LIBRARIES}
        PUBLIC stlab )

target_link_libraries( stlab.test.channel.test PUBLIC
        "${stlab_base_flags}" )

target_link_libraries( stlab.test.channel.test PUBLIC
        "$<$<CONFIG:DEBUG>:${stlab_debug_flags}>" )

target_link_libraries( stlab.test.channel.test PUBLIC
        "$<$<CONFIG:DEBUG>:$<$<BOOL:${coverage}>:${stlab_coverage_flags}>>" )

target_link_libraries( stlab.test.channel.test PUBLIC
        "$<$<CONFIG:RELEASE>:${stlab_release_flags}>" )

target_link_libraries( stlab.test.channel.test PUBLIC
        "${stlab_appended_flags}" )

set_property(TARGET stlab.test.channel.test PROPERTY CXX_STANDARD 14)
set_property(TARGET stlab.test.channel.test PROPERTY CXX_STANDARD_REQUIRED ON)

add_test( NAME stlab.test.channel COMMAND stlab.test.channel.test )


####################################################################################################

add_executable( stlab.test.future.test
                "${CMAKE_CURRENT_SOURCE_DIR}/future_coroutine_tests.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/future_recover_tests.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/future_test_helper.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/future_tests.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/future_then_tests.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/future_when_all_arguments_tests.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/future_when_all_range_tests.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/future_when_any_arguments_tests.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/future_when_any_range_tests.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/tuple_algorithm_test.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/future_test_helper.hpp" )

# This syntax is a CMake generator expression.
#
# Here we defer the compiler flags decision-making to generation time
# (for tools which support distinct configuration and generation steps)
# based on a generation time configuration.
#
# Here, we provide support for a DEBUG and RELEASE configurations, line 32
# providing the former and line 33 providing the latter.
target_compile_options( stlab.test.future.test
                        PUBLIC ${stlab_base_flags}
                               $<$<CONFIG:DEBUG>:$<$<BOOL:${coverage}>:${stlab_coverage_flags}>${stlab_debug_flags}>
                               $<$<BOOL:${coroutine}>:${stlab_coroutine_flags}>
                               $<$<CONFIG:RELEASE>:${stlab_release_flags}> 
                               ${stlab_appended_flags} )

target_link_libraries( stlab.test.future.test
                       PUBLIC ${Boost_LIBRARIES} 
                       PUBLIC stlab )
                     
target_link_libraries( stlab.test.future.test PUBLIC
                       "${stlab_base_flags}" )

target_link_libraries( stlab.test.future.test PUBLIC
                       "$<$<CONFIG:DEBUG>:${stlab_debug_flags}>" )

target_link_libraries( stlab.test.future.test PUBLIC
                       "$<$<CONFIG:DEBUG>:$<$<BOOL:${coverage}>:${stlab_coverage_flags}>>" )

target_link_libraries( stlab.test.future.test PUBLIC
                       "$<$<CONFIG:RELEASE>:${stlab_release_flags}>" ) 
                             
target_link_libraries( stlab.test.future.test PUBLIC
                       "${stlab_appended_flags}" )

set_property(TARGET stlab.test.future.test PROPERTY CXX_STANDARD 14)
set_property(TARGET stlab.test.future.test PROPERTY CXX_STANDARD_REQUIRED ON)
                     
add_test( NAME stlab.test.future COMMAND stlab.test.future.test )

####################################################################################################

add_executable( stlab.test.serial_queue.test
                "${CMAKE_CURRENT_SOURCE_DIR}/serial_queue_test.cpp" )

target_compile_options( stlab.test.serial_queue.test
                        PUBLIC "${stlab_base_flags}"
                               $<$<CONFIG:DEBUG>:$<$<BOOL:${coverage}>:${stlab_coverage_flags}>${stlab_debug_flags}>
                               $<$<CONFIG:RELEASE>:${stlab_release_flags}> 
                               ${stlab_appended_flags} )

target_link_libraries( stlab.test.serial_queue.test
                       PUBLIC ${Boost_LIBRARIES} 
                       PUBLIC stlab )
                             
target_link_libraries( stlab.test.serial_queue.test PUBLIC
                       "${stlab_base_flags}" )

target_link_libraries( stlab.test.serial_queue.test PUBLIC
                       "$<$<CONFIG:DEBUG>:${stlab_debug_flags}>" )

target_link_libraries( stlab.test.serial_queue.test PUBLIC
                       "$<$<CONFIG:DEBUG>:$<$<BOOL:${coverage}>:${stlab_coverage_flags}>>" )

target_link_libraries( stlab.test.serial_queue.test PUBLIC
                       "$<$<CONFIG:RELEASE>:${stlab_release_flags}>" ) 
                             
target_link_libraries( stlab.test.serial_queue.test PUBLIC
                       "${stlab_appended_flags}" )
                             
set_property(TARGET stlab.test.serial_queue.test PROPERTY CXX_STANDARD 14)
set_property(TARGET stlab.test.serial_queue.test PROPERTY CXX_STANDARD_REQUIRED ON)
                     
add_test( NAME stlab.test.serial_queue COMMAND stlab.test.serial_queue.test )

####################################################################################################

add_executable( stlab.test.cow.test 
                "${CMAKE_CURRENT_SOURCE_DIR}/cow_test.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp" )

target_compile_options( stlab.test.cow.test
                        PUBLIC "${stlab_base_flags}"
                               $<$<CONFIG:DEBUG>:$<$<BOOL:${coverage}>:${stlab_coverage_flags}>${stlab_debug_flags}>
                               $<$<CONFIG:RELEASE>:${stlab_release_flags}>
                               ${stlab_appended_flags} )

target_link_libraries( stlab.test.cow.test 
                       PUBLIC ${Boost_LIBRARIES} 
                       PUBLIC stlab )
                             
target_link_libraries( stlab.test.cow.test PUBLIC
                       "${stlab_base_flags}" )

target_link_libraries( stlab.test.cow.test PUBLIC
                       "$<$<CONFIG:DEBUG>:${stlab_debug_flags}>" )

target_link_libraries( stlab.test.cow.test PUBLIC
                       "$<$<CONFIG:DEBUG>:$<$<BOOL:${coverage}>:${stlab_coverage_flags}>>" )

target_link_libraries( stlab.test.cow.test PUBLIC
                       "$<$<CONFIG:RELEASE>:${stlab_release_flags}>" ) 
                             
target_link_libraries( stlab.test.cow.test PUBLIC
                       "${stlab_appended_flags}" )
                             
set_property(TARGET stlab.test.cow.test PROPERTY CXX_STANDARD 14)
set_property(TARGET stlab.test.cow.test PROPERTY CXX_STANDARD_REQUIRED ON)

add_test( NAME stlab.test.cow COMMAND stlab.test.cow.test )

####################################################################################################

add_executable( stlab.test.task.test 
                "${CMAKE_CURRENT_SOURCE_DIR}/task_test.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp" )

target_compile_options( stlab.test.task.test
                        PUBLIC "${stlab_base_flags}"
                               $<$<CONFIG:DEBUG>:$<$<BOOL:${coverage}>:${stlab_coverage_flags}>${stlab_debug_flags}>
                               $<$<CONFIG:RELEASE>:${stlab_release_flags}>
                               ${stlab_appended_flags} )

target_link_libraries( stlab.test.task.test 
                       PUBLIC ${Boost_LIBRARIES} 
                       PUBLIC stlab )
                             
target_link_libraries( stlab.test.task.test PUBLIC
                       "${stlab_base_flags}" )

target_link_libraries( stlab.test.task.test PUBLIC
                       "$<$<CONFIG:DEBUG>:${stlab_debug_flags}>" )

target_link_libraries( stlab.test.task.test PUBLIC
                       "$<$<CONFIG:DEBUG>:$<$<BOOL:${coverage}>:${stlab_coverage_flags}>>" )

target_link_libraries( stlab.test.task.test PUBLIC
                       "$<$<CONFIG:RELEASE>:${stlab_release_flags}>" ) 
                             
target_link_libraries( stlab.test.task.test PUBLIC
                       "${stlab_appended_flags}" )
                             
set_property(TARGET stlab.test.task.test PROPERTY CXX_STANDARD 14)
set_property(TARGET stlab.test.task.test PROPERTY CXX_STANDARD_REQUIRED ON)

add_test( NAME stlab.test.task COMMAND stlab.test.task.test )

####################################################################################################

add_executable( stlab.test.tuple.test 
                "${CMAKE_CURRENT_SOURCE_DIR}/tuple_test.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp" )

target_compile_options( stlab.test.tuple.test
                        PUBLIC "${stlab_base_flags}"
                               $<$<CONFIG:DEBUG>:$<$<BOOL:${coverage}>:${stlab_coverage_flags}>${stlab_debug_flags}>
                               $<$<CONFIG:RELEASE>:${stlab_release_flags}>
                               ${stlab_appended_flags} )

target_link_libraries( stlab.test.tuple.test 
                       PUBLIC ${Boost_LIBRARIES} 
                       PUBLIC stlab )
                             
target_link_libraries( stlab.test.tuple.test PUBLIC
                       "${stlab_base_flags}" )

target_link_libraries( stlab.test.tuple.test PUBLIC
                       "$<$<CONFIG:DEBUG>:${stlab_debug_flags}>" )

target_link_libraries( stlab.test.tuple.test PUBLIC
                       "$<$<CONFIG:DEBUG>:$<$<BOOL:${coverage}>:${stlab_coverage_flags}>>" )

target_link_libraries( stlab.test.tuple.test PUBLIC
                       "$<$<CONFIG:RELEASE>:${stlab_release_flags}>" ) 
                             
target_link_libraries( stlab.test.tuple.test PUBLIC
                       "${stlab_appended_flags}" )
                             
set_property(TARGET stlab.test.tuple.test PROPERTY CXX_STANDARD 14)
set_property(TARGET stlab.test.tuple.test PROPERTY CXX_STANDARD_REQUIRED ON)

add_test( NAME stlab.test.tuple COMMAND stlab.test.tuple.test )

