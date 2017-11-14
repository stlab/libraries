set( Boost_MULTITHREADED ON ) 
set( Boost_USE_STATIC_LIBS ON )
if( EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake )
    include( ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake )
    set( CONAN_SYSTEM_INCLUDES ON )
    conan_basic_setup()
else()
    find_package( Boost REQUIRED COMPONENTS unit_test_framework)
endif()


################################################################################
# Test function
################################################################################
function(stlab_test target_name target_file)
    add_executable(${target_name} ${target_file})
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
stlab_test( stlab.test.main "main.cpp" )
stlab_test( stlab.test.tuple "tuple_test.cpp" )
stlab_test( stlab.test.tuple.algorithm "tuple_algorithm_test.cpp" )
stlab_test( stlab.test.task "task_test.cpp" )
stlab_test( stlab.test.cow "cow_test.cpp" )
stlab_test( stlab.test.serial_queue "serial_queue_test.cpp" )

stlab_test( stlab.test.channel channel_functor_tests.cpp 
                               channel_join_tests.cpp 
                               channel_merge_tests.cpp 
                               channel_process_tests.cpp 
                               channel_zip_tests.cpp 
                               channel_test_helper.cpp
                               channel_tests.cpp )

                                     #stlab_future_test( stlab.test.future.recover "future_recover_tests.cpp" )
                                     #stlab_future_test( stlab.test.future.then "future_then_tests.cpp" )
                                     #stlab_future_test( stlab.test.future.when.all.arguments "future_when_all_arguments_tests.cpp" )
                                     #stlab_future_test( stlab.test.future.when.all.range "future_when_all_range_tests.cpp" )
                                     #stlab_future_test( stlab.test.future.when.any.arguments "future_when_any_arguments_tests.cpp" )
                                     #stlab_future_test( stlab.test.future.when.any.range "future_when_any_range_tests.cpp" )
                                     #stlab_future_test( stlab.test.future "future_tests.cpp" )
                                     #stlab_future_test( stlab.test.future.helper "future_test_helper.cpp" )
