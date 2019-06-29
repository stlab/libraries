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

BOOST_AUTO_TEST_CASE(all_low_prio_tasks_are_executed) {
    BOOST_TEST_MESSAGE("All low priority tasks are executed");

    serial_queue_t queue(low_executor);
    std::vector<int> results;
    std::atomic_bool done{false};

    for (auto i = 0; i < 10; ++i) {
        queue.executor()([_i = i, &results] { results.push_back(_i); });
    }
    queue.executor()([&done] { done = true; });

    while (!done) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    for (auto i = 0; i < 10; ++i) {
        BOOST_REQUIRE_EQUAL(i, results[i]);
    }
}

BOOST_AUTO_TEST_CASE(all_default_prio_tasks_get_executed) {
    BOOST_TEST_MESSAGE("All default priority tasks are executed");

    serial_queue_t queue(default_executor);
    std::vector<int> results;
    std::atomic_bool done{false};

    for (auto i = 0; i < 10; ++i) {
        queue.executor()([_i = i, &results] { results.push_back(_i); });
    }
    queue.executor()([&done] { done = true; });

    while (!done) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    for (auto i = 0; i < 10; ++i) {
        BOOST_REQUIRE_EQUAL(i, results[i]);
    }
}

BOOST_AUTO_TEST_CASE(all_high_prio_tasks_get_executed) {
    BOOST_TEST_MESSAGE("All high priority tasks are executed");

    serial_queue_t queue(high_executor);
    std::vector<int> results;
    std::atomic_bool done{false};

    for (auto i = 0; i < 10; ++i) {
        queue.executor()([_i = i, &results] { results.push_back(_i); });
    }
    queue.executor()([&done] { done = true; });

    while (!done) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    for (auto i = 0; i < 10; ++i) {
        BOOST_REQUIRE_EQUAL(i, results[i]);
    }
}

#if STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_PORTABLE

BOOST_AUTO_TEST_CASE(all_tasks_will_be_executed_according_to_their_prio) {
    BOOST_TEST_MESSAGE("All tasks will be executed according to their prio");

    std::mutex m;
    std::vector<int> results;
    const auto iterations = 30000;
    results.reserve(iterations * 3);

    {
        std::unique_lock block{m};
        for (auto i = 0; i < iterations; ++i) {
            low_executor([&] {
                std::unique_lock guard{m};
                results.push_back(1);
            });

            default_executor([&] {
                std::unique_lock guard{m};
                results.push_back(2);
            });
            high_executor([&] {
                std::unique_lock guard{m};
                results.push_back(3);
            });
        }
    }

    while (true) {
        {
            std::unique_lock guard{m};
            if (results.size() == iterations * 3) break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    auto highSum{0};
    auto defaultSum{0};
    auto lowSum{0};
    for (auto i = 0; i < iterations; ++i) {
        highSum += results[i];
        defaultSum += results[i + iterations];
        lowSum += results[i + iterations * 2];
    }

    BOOST_WARN_GT(0.01, std::fabs(3.0 - static_cast<double>(highSum) / iterations));
    BOOST_WARN_GT(0.01, std::fabs(2.0 - static_cast<double>(defaultSum) / iterations));
    BOOST_WARN_GT(0.01, std::fabs(1.0 - static_cast<double>(lowSum) / iterations));
}

#endif


BOOST_AUTO_TEST_CASE(MeasureTiming) {
  std::vector<int> results;
  const auto iterations = 30000;
  results.resize(iterations * 3);
  std::mutex m;
  std::condition_variable cv;
  bool done = false;
  default_executor([]{});
  auto start = std::chrono::steady_clock::now();

    for (auto i = 0; i < iterations; ++i) {
      low_executor([_i = i, &results] {
        results[_i] = 1;
      });

      default_executor([_i = i+iterations, &results] {
        results[_i] = 2;
      });
      high_executor([_i = i + iterations*2, &results]{
        results[_i] = 3;
      });
    }
    low_executor([&cv,&done] {
      {
        done = true;
      }
      cv.notify_one();
    });

    std::unique_lock block{ m };
    while (!done)
    {
      cv.wait(block);
    }

    auto stop = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration<double>(stop - start).count() << "\n";

}
