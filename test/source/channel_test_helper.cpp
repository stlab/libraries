/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#include "channel_test_helper.hpp"

std::queue<stlab::task<void()>> channel_test_helper::manual_scheduler::_tasks;
std::mutex channel_test_helper::manual_scheduler::_mutex;

int channel_test_helper::timed_sum::_x{0};
std::mutex channel_test_helper::timed_sum::_mutex;
