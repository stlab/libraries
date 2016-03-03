
add_executable(future_test
  future_recover_tests.cpp
  future_tests.cpp
  future_then_tests.cpp
  future_when_all_arguments_test.cpp
  future_when_all_range_tests.cpp
  future_when_any_tests.cpp
  main.cpp
)


#add_dependencies(future_test ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

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
