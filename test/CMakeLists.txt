if( EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake )
    include( ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake )
    set( CONAN_SYSTEM_INCLUDES ON )
    conan_basic_setup()
endif()
set( Boost_MULTITHREADED ON ) 
set( Boost_USE_STATIC_LIBS ON )
find_package( Boost REQUIRED COMPONENTS unit_test_framework)


################################################################################
# Test function
################################################################################
function(stlab_test target_name)
    add_executable(${target_name} ${ARGN})
    target_link_libraries(${target_name} PRIVATE 
      stlab 
      Boost::unit_test_framework)
  add_test(NAME ${target_name}
      WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
      COMMAND ${target_name})
  if ( NOT MSVC )
      target_compile_options(${target_name} PRIVATE "-Wall;-ftemplate-backtrace-limit=0;")
  endif()
  target_compile_definitions(${target_name} PRIVATE BOOST_TEST_MODULE=stlab_libraries_tests)
endfunction()


################################################################################
# Tests
################################################################################
stlab_test( stlab.test.tuple tuple_test.cpp )
stlab_test( stlab.test.tuple.algorithm tuple_algorithm_test.cpp)
stlab_test( stlab.test.task task_test.cpp )
stlab_test( stlab.test.cow cow_test.cpp )
stlab_test( stlab.test.serial_queue serial_queue_test.cpp )

stlab_test( stlab.test.channel_functor channel_functor_tests.cpp)
stlab_test( stlab.test.channel_join channel_join_tests.cpp)
stlab_test( stlab.test.channel_merge channel_merge_tests.cpp)
stlab_test( stlab.test.channel_process channel_process_tests.cpp)
stlab_test( stlab.test.channel_zip channel_zip_tests.cpp)
stlab_test( stlab.test.channel channel_tests.cpp)

stlab_test( stlab.test.future_recover future_recover_tests.cpp ) 
stlab_test( stlab.test.future future_tests.cpp )
stlab_test( stlab.test.future_then future_then_tests.cpp )
stlab_test( stlab.test.future_when_all_arguments future_when_all_arguments_tests.cpp )
stlab_test( stlab.test.future_when_all_range future_when_all_range_tests.cpp )
stlab_test( stlab.test.future_when_any_arguments future_when_any_arguments_tests.cpp ) 
stlab_test( stlab.test.future_when_any_range future_when_any_range_tests.cpp ) 
