/*
    Copyright 2013 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
/**************************************************************************************************/

#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/serial_queue.hpp>
#include <boost/test/unit_test.hpp>

#include <thread>

using namespace stlab;

BOOST_AUTO_TEST_CASE(all_low_prio_tasks_get_executed) {

    serial_queue_t queue(low_executor);
    std::vector<int> results;
    std::atomic_bool done{false};

    for (auto i = 0; i < 10; ++i)
    {
        queue.executor()([_i = i, &results]{
            results.push_back(_i);
        });
    }
    queue.executor()([&done]{
        done = true;
    });

    while (!done) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    for (auto i = 0; i < 10; ++i)
    {
        BOOST_REQUIRE_EQUAL(i, results[i]);
    }
}

BOOST_AUTO_TEST_CASE(all_default_prio_tasks_get_executed) {

    serial_queue_t queue(default_executor);
    std::vector<int> results;
    std::atomic_bool done{false};

    for (auto i = 0; i < 10; ++i)
    {
        queue.executor()([_i = i, &results]{
            results.push_back(_i);
        });
    }
    queue.executor()([&done]{
        done = true;
    });

    while (!done) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    for (auto i = 0; i < 10; ++i)
    {
        BOOST_REQUIRE_EQUAL(i, results[i]);
    }
}

BOOST_AUTO_TEST_CASE(all_high_prio_tasks_get_executed) {

    serial_queue_t queue(high_executor);
    std::vector<int> results;
    std::atomic_bool done{false};

    for (auto i = 0; i < 10; ++i)
    {
        queue.executor()([_i = i, &results]{
            results.push_back(_i);
        });
    }
    queue.executor()([&done]{
        done = true;
    });

    while (!done) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    for (auto i = 0; i < 10; ++i)
    {
        BOOST_REQUIRE_EQUAL(i, results[i]);
    }
}

