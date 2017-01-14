/*
Copyright 2016 Felix Petriconi
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#include "channel_test_helper.hpp"

std::queue<std::function<void()>> manual_scheduler::_tasks;
std::atomic_int timed_sum::_x{ 0 };