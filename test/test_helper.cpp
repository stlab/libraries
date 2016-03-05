/*
Copyright 2016 Felix Petriconi
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#include "test_helper.hpp"

std::atomic_int test_helper::custom_scheduler<0>::_usage_counter = 0;
std::atomic_int test_helper::custom_scheduler<1>::_usage_counter = 0;

