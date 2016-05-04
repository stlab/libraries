/*
Copyright 2016 Felix Petriconi
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#include "test_helper.hpp"
using namespace test_helper;

template<>
std::atomic_int custom_scheduler<0>::_usage_counter{ 0 };
template<>
std::atomic_int custom_scheduler<1>::_usage_counter{ 0 };
template<>
std::atomic_int custom_scheduler<0>::_current_tasks_in_execution{ 0 };
template<>
std::atomic_int custom_scheduler<1>::_current_tasks_in_execution{ 0 };

test_exception::test_exception(const std::string& error)
    : _error(error)
{}

test_exception::test_exception(const char* error)
    : _error(error)
{}

const char* test_exception::what() const noexcept {
    return _error.c_str();
}
