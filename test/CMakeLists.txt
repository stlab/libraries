PROJECT(future_test)

include (LCoverage)

set(SOURCE
  channel_functor_tests.cpp
  channel_join_tests.cpp
  channel_merge_tests.cpp
  channel_process_tests.cpp
  channel_tests.cpp
  channel_test_helper.cpp
  channel_zip_tests.cpp
  future_recover_tests.cpp
  future_tests.cpp
  future_then_tests.cpp
  future_when_all_arguments_tests.cpp
  future_when_all_range_tests.cpp
  future_when_any_arguments_tests.cpp
  future_when_any_range_tests.cpp
  test_helper.cpp
  tuple_algorithm_test.cpp
  main.cpp
)

set(HEADERS
  ../stlab/concurrency/channel.hpp
  ../stlab/concurrency/config.hpp
  ../stlab/concurrency/default_executor.hpp
  ../stlab/concurrency/executor_base.hpp
  ../stlab/concurrency/future.hpp
  ../stlab/concurrency/immediate_executor.hpp
  ../stlab/concurrency/main_executor.hpp
  ../stlab/concurrency/progress.hpp
  ../stlab/concurrency/system_timer.hpp
  ../stlab/concurrency/traits.hpp
  ../stlab/concurrency/tuple_algorithm.hpp
  ../stlab/concurrency/utility.hpp
  channel_test_helper.hpp
  test_helper.hpp
)

add_executable(future_test ${SOURCE} ${HEADERS})

include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(
    future_test
    ${Boost_LIBRARIES} 
    ${CMAKE_THREAD_LIBS_INIT} 
    ${CONAN_LIBS})

set_property(TARGET future_test PROPERTY CXX_STANDARD 14)
set_property(TARGET future_test PROPERTY CXX_STANDARD_REQUIRED ON)

########## Serial queue ##########

PROJECT(serial_queue_test)

set(SOURCE
    serial_queue_test.cpp
)

set(HEADERS
)

add_executable(serial_queue_test ${SOURCE} ${HEADERS})

include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(
    serial_queue_test
    ${Boost_LIBRARIES} 
    ${CMAKE_THREAD_LIBS_INIT} 
    ${CONAN_LIBS})

set_property(TARGET serial_queue_test PROPERTY CXX_STANDARD 14)
set_property(TARGET serial_queue_test PROPERTY CXX_STANDARD_REQUIRED ON)

setup_target_for_coverage(NAME serial_queue_test_coverage EXECUTABLE serial_queue_test)

########## Copy On Write ##########

PROJECT(cow_test)

set(SOURCE
    cow_test.cpp
    main.cpp
)

set(HEADERS
)

add_executable(cow_test ${SOURCE} ${HEADERS})

include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(
    cow_test
    ${Boost_LIBRARIES} 
    ${CMAKE_THREAD_LIBS_INIT} 
    ${CONAN_LIBS})

set_property(TARGET cow_test PROPERTY CXX_STANDARD 14)
set_property(TARGET cow_test PROPERTY CXX_STANDARD_REQUIRED ON)
