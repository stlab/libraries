/*
Copyright 2016 Felix Petriconi
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <stlab/future.hpp>
#include <stlab/progress.hpp>
#include <iostream>

#include "test_helper.hpp"

using namespace stlab;
using namespace test_helper;
#if 0
BOOST_AUTO_TEST_CASE(PassivProgressExample)
{
    stlab::progress_tracker pt1;

    auto f1 = async(custom_scheduler<0>(), pt1([&_pt = pt1] { std::cout << _pt.completed() << std::endl; return 42; }))
        .then(pt1([&_pt = pt1](int x) { std::cout << _pt.completed() << std::endl; return x + 42; }));

    while (!f1.get_try());
}

BOOST_AUTO_TEST_CASE(ActiveProgressExample)
{
    auto progress_callback = [](size_t task_number, size_t done_tasks) {
        std::cout << done_tasks << "/" << task_number << " tasks performed." << std::endl;
    };
    stlab::progress_tracker pt2(progress_callback);

    auto f2 = async(custom_scheduler<0>(), pt2([] { return 42; }))
        .then(pt2([](int x) { return x + 42; }));

    while (!f2.get_try());

    std::cout << "Result: " << *f2.get_try() << std::endl;
}
#endif