PROJECT(future_test)

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
  channel_test_helper.hpp
  test_helper.hpp
)

add_executable(future_test ${SOURCE} ${HEADERS})

include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(
    future_test
    future
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT})

set_property(TARGET future_test PROPERTY CXX_STANDARD 14)
set_property(TARGET future_test PROPERTY CXX_STANDARD_REQUIRED ON)
