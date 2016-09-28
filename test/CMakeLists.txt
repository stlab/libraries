PROJECT(future_test)

set(SOURCE
  future_recover_tests.cpp
  future_tests.cpp
  future_then_tests.cpp
  future_when_all_arguments_tests.cpp
  future_when_all_range_tests.cpp
  future_when_any_arguments_tests.cpp
  future_when_any_range_tests.cpp
  test_helper.cpp
  main.cpp
)

set(HEADERS
  test_helper.hpp
)

add_executable(future_test ${SOURCE} ${HEADERS})

include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(
    future_test
    future
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${CMAKE_THREAD_LIBS_INIT})


set_property(TARGET future_test PROPERTY CXX_STANDARD 14)
set_property(TARGET future_test PROPERTY CXX_STANDARD_REQUIRED ON)
