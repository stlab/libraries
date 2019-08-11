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

namespace
{
inline void rest() { std::this_thread::sleep_for(std::chrono::milliseconds(1)); }
}

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
        rest();
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
        rest();
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
        rest();
    }

    for (auto i = 0; i < 10; ++i) {
        BOOST_REQUIRE_EQUAL(i, results[i]);
    }
}

BOOST_AUTO_TEST_CASE(task_system_restarts_after_it_went_pending) {
    BOOST_TEST_MESSAGE("The task system restarts after it went to pending");

    atomic_bool done{false};
    mutex m;
    condition_variable cv;

    default_executor([&]{
        rest();
        {
            unique_lock block{m};
            done = true;
        }
        cv.notify_one();
    });


    {
        unique_lock block{m};
        while (!done) {
            cv.wait(block);
        }
    }

    default_executor([&] {
        rest();
        {
            unique_lock block{m};
            done = false;
            cv.notify_one();
        }
    });

    {
        unique_lock block{m};
        while (done) {
            cv.wait(block);
        }
    }

    BOOST_REQUIRE(!done);
}

BOOST_AUTO_TEST_CASE(all_tasks_will_be_executed_according_to_their_prio) {
    BOOST_TEST_MESSAGE("All tasks will be executed according to their prio");

    const auto iterations = 300000;
    std::atomic_int done{0};
    std::atomic_int highCount{0};
    std::atomic_int defaultCount{0};
    std::atomic_int correctDefault{0};
    std::atomic_int correctLow{0};
    std::atomic_int taskRunning{0};
    {
        for (auto i = 0; i < iterations; ++i) {
            low_executor([&] {
                ++taskRunning;
                correctLow += static_cast<int>(highCount <= taskRunning && defaultCount <= taskRunning);
                ++done;
                --taskRunning;
            });
            ++defaultCount;
            default_executor([&] {
                ++taskRunning;
                correctDefault += static_cast<int>(highCount <= taskRunning);
                --defaultCount;
                ++done;
                --taskRunning;
            });
            ++highCount;
            high_executor([&] {
                ++taskRunning;
                --highCount;
                ++done;
                --taskRunning;
            });
        }
    }

    while (done != iterations * 3) {
        rest();
    }

    cout << "Correct default ordering: " << static_cast<double>(correctDefault.load())/iterations <<"%\n";
    cout << "Correct low ordering:     " << static_cast<double>(correctLow.load())/iterations << "%\n";
}

BOOST_AUTO_TEST_CASE(MeasureTiming) {
    std::vector<int> results;
    const auto iterations = 100000;
    results.resize(iterations * 3);
    atomic_bool done = false;
    condition_variable ready;
    atomic_int counter{0};

    auto start = chrono::high_resolution_clock::now();

    for (auto i = 0; i < iterations; ++i) {
        low_executor([_i = i, &results,&counter] {
            results[_i] = 1;
            ++counter;
        });
        default_executor([_i = i + iterations, &results,&counter] {
            results[_i] = 2;
            ++counter;
        });
        high_executor([_i = i + iterations * 2, &results,&counter] {
            results[_i] = 3;
            ++counter;
        });
    }

    mutex block;
    low_executor([&] {
        {
            unique_lock lock{block};
            done = true;

            ready.notify_one();
        }
    });


    unique_lock lock{block};
    while (!done) ready.wait(lock);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout << "\nPerformance measuring: " << std::chrono::duration<double>(stop - start).count() << "s\n";

    while (counter < 3*iterations) {
        rest();
    }
}
