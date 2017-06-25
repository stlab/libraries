add_executable( stlab.test.future.test
                "${CMAKE_CURRENT_SOURCE_DIR}/channel_functor_tests.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/channel_join_tests.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/channel_merge_tests.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/channel_process_tests.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/channel_tests.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/channel_test_helper.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/channel_zip_tests.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/future_recover_tests.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/future_tests.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/future_then_tests.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/future_when_all_arguments_tests.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/future_when_all_range_tests.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/future_when_any_arguments_tests.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/future_when_any_range_tests.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/test_helper.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/tuple_algorithm_test.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/channel_test_helper.hpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/test_helper.hpp" )

target_link_libraries( stlab.test.future.test
                       PUBLIC ${Boost_LIBRARIES} 
                       PUBLIC stlab )

target_compile_options( stlab.test.future.test
                        PRIVATE $<$<CONFIG:DEBUG>:$<$<BOOL:${coverage}>:${stlab_coverage_flags}>${stlab_debug_flags}>
                                $<$<CONFIG:RELEASE>:${stlab_release_flags}> 
                                ${stlab_appended_flags} )
                              
set_property(TARGET stlab.test.future.test PROPERTY CXX_STANDARD 14)
set_property(TARGET stlab.test.future.test PROPERTY CXX_STANDARD_REQUIRED ON)
                     
add_test( NAME stlab.test.future COMMAND stlab.test.future.test )

add_executable( stlab.test.serial_queue.test
                "${CMAKE_CURRENT_SOURCE_DIR}/serial_queue_test.cpp" )

target_link_libraries( stlab.test.serial_queue.test
                       PUBLIC ${Boost_LIBRARIES} 
                       PUBLIC stlab )

target_compile_options( stlab.test.serial_queue.test
                        PRIVATE $<$<CONFIG:DEBUG>:$<$<BOOL:${coverage}>:${stlab_coverage_flags}>${stlab_debug_flags}>
                                $<$<CONFIG:RELEASE>:${stlab_release_flags}> 
                                ${stlab_appended_flags} )
                              
set_property(TARGET stlab.test.serial_queue.test PROPERTY CXX_STANDARD 14)
set_property(TARGET stlab.test.serial_queue.test PROPERTY CXX_STANDARD_REQUIRED ON)
                     
add_test( NAME stlab.test.serial_queue COMMAND stlab.test.serial_queue.test )

add_executable( stlab.test.cow.test 
                "${CMAKE_CURRENT_SOURCE_DIR}/cow_test.cpp"
                "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp" )

target_link_libraries( stlab.test.cow.test 
                       PUBLIC ${Boost_LIBRARIES} 
                       PUBLIC stlab )

target_compile_options( stlab.test.cow.test
                        PRIVATE $<$<CONFIG:DEBUG>:$<$<BOOL:${coverage}>:${stlab_coverage_flags}>${stlab_debug_flags}>
                                $<$<CONFIG:RELEASE>:${stlab_release_flags}>
                                ${stlab_appended_flags} )
                     
set_property(TARGET stlab.test.cow.test PROPERTY CXX_STANDARD 14)
set_property(TARGET stlab.test.cow.test PROPERTY CXX_STANDARD_REQUIRED ON)

add_test( NAME stlab.test.cow COMMAND stlab.test.cow.test )

