/*
    Copyright 2013 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
/**************************************************************************************************/

#include <boost/test/unit_test.hpp>

#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/serial_queue.hpp>

#include <cmath>
#include <iostream>
#include <thread>

using namespace stlab;
using namespace std;

BOOST_AUTO_TEST_CASE(all_low_prio_tasks_are_executed) {
    BOOST_TEST_MESSAGE("All low priority tasks are executed");

    serial_queue_t queue(low_executor);
    mutex m;
    vector<int> results;
    atomic_bool done{false};

    for (auto i = 0; i < 10; ++i) {
        queue.executor()([_i = i, &m, &results] {
            unique_lock block{m};
            results.push_back(_i);
        });
    }
    queue.executor()([&done] { done = true; });

    while (!done) {
        this_thread::sleep_for(chrono::milliseconds(1));
    }

    for (auto i = 0; i < 10; ++i) {
        BOOST_REQUIRE_EQUAL(i, results[i]);
    }
}

BOOST_AUTO_TEST_CASE(all_default_prio_tasks_get_executed) {
    BOOST_TEST_MESSAGE("All default priority tasks are executed");

    serial_queue_t queue(default_executor);
    mutex m;
    vector<int> results;
    atomic_bool done{false};

    for (auto i = 0; i < 10; ++i) {
        queue.executor()([_i = i, &m, &results] {
            unique_lock block{m};
            results.push_back(_i);
        });
    }
    queue.executor()([&done] { done = true; });

    while (!done) {
        this_thread::sleep_for(chrono::milliseconds(1));
    }

    for (auto i = 0; i < 10; ++i) {
        BOOST_REQUIRE_EQUAL(i, results[i]);
    }
}

BOOST_AUTO_TEST_CASE(all_high_prio_tasks_get_executed) {
    BOOST_TEST_MESSAGE("All high priority tasks are executed");

    serial_queue_t queue(high_executor);
    mutex m;
    vector<int> results;
    atomic_bool done{false};

    for (auto i = 0; i < 10; ++i) {
        queue.executor()([_i = i, &m, &results] {
            unique_lock block{m};
            results.push_back(_i);
        });
    }
    queue.executor()([&done] { done = true; });

    while (!done) {
        this_thread::sleep_for(chrono::milliseconds(1));
    }

    for (auto i = 0; i < 10; ++i) {
        BOOST_REQUIRE_EQUAL(i, results[i]);
    }
}

BOOST_AUTO_TEST_CASE(all_tasks_will_be_executed_according_to_their_prio) {
    BOOST_TEST_MESSAGE("All tasks will be executed according to their prio");

    mutex m;
    vector<int> results;
    const auto iterations = 30000;
    results.reserve(iterations * 3);

    {
        unique_lock block{m};
        for (auto i = 0; i < iterations; ++i) {
            low_executor([&] {
                unique_lock guard{m};
                results.push_back(1);
            });
            default_executor([&] {
                unique_lock guard{m};
                results.push_back(2);
            });
            high_executor([&] {
                unique_lock guard{m};
                results.push_back(3);
            });
        }
    }

    while (true) {
        {
            unique_lock guard{m};
            if (results.size() == iterations * 3) break;
        }
        this_thread::sleep_for(chrono::milliseconds(1));
    }

    auto highSum{0};
    auto defaultSum{0};
    auto lowSum{0};
    for (auto i = 0; i < iterations; ++i) {
        highSum += results[i];
        defaultSum += results[i + iterations];
        lowSum += results[i + iterations * 2];
    }

    auto correctHighOrdering = fabs(3.0 - static_cast<double>(highSum) / iterations);
    auto correctDefaultOrdering = fabs(2.0 - static_cast<double>(defaultSum) / iterations);
    auto correctLowOrdering = fabs(1.0 - static_cast<double>(lowSum) / iterations);

    cout << "Correct high ordering " << 1.0 - correctHighOrdering << "%\n";
    cout << "Correct default ordering " << 1.0 - correctDefaultOrdering << "%\n";
    cout << "Correct low ordering " << 1.0 - correctLowOrdering << "%\n";

    BOOST_WARN_GT(0.01, correctHighOrdering);
    BOOST_WARN_GT(0.01, correctDefaultOrdering);
    BOOST_WARN_GT(0.01, correctLowOrdering);
}

BOOST_AUTO_TEST_CASE(MeasureTiming) {
    std::vector<int> results;
    const auto iterations = 30000;
    results.resize(iterations * 3);
    bool done = false;
    condition_variable ready;

    auto start = chrono::high_resolution_clock::now();

    for (auto i = 0; i < iterations; ++i) {
        low_executor([_i = i, &results] {
            results[_i] = 1;
        });
        default_executor([_i = i + iterations, &results] {
            results[_i] = 2;
        });
        high_executor([_i = i + iterations * 2, &results] {
            results[_i] = 3;
        });
    }

    low_executor([&]{ done = true; ready.notify_one(); });

    mutex block;
    unique_lock<mutex> lock{block};
    while (!done) ready.wait(lock);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout << "Complete calculation time:" << std::chrono::duration<double>(stop - start).count() << "s\n";

    // we have to be sure, that really all tasks are done
    this_thread::sleep_for(chrono::milliseconds(100));
}
