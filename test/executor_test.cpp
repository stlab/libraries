/*
    Copyright 2013 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
/**************************************************************************************************/

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/test/unit_test.hpp>

#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/serial_queue.hpp>

#include <array>
#include <cmath>
#include <condition_variable>
#include <iostream>
#include <thread>

using namespace stlab;
using namespace std;

// Use 'mpre' instead of 'mp' because the latter conflicts with Boost.Math <= 1.76
namespace mpre = boost::multiprecision;

namespace {
void rest() { std::this_thread::sleep_for(std::chrono::milliseconds(1)); }

template <typename T, typename N, typename O>
T power(T x, N n, O op) {
    if (n == 0) return identity_element(op);

    while ((n & 1) == 0) {
        n >>= 1;
        x = op(x, x);
    }

    T result = x;
    n >>= 1;
    while (n != 0) {
        x = op(x, x);
        if ((n & 1) != 0) result = op(result, x);
        n >>= 1;
    }
    return result;
}

template <typename N>
struct multiply_2x2 {
    std::array<N, 4> operator()(const std::array<N, 4>& x, const std::array<N, 4>& y) {
        return {x[0] * y[0] + x[1] * y[2], x[0] * y[1] + x[1] * y[3], x[2] * y[0] + x[3] * y[2],
                x[2] * y[1] + x[3] * y[3]};
    }
};
template <typename N>
std::array<N, 4> identity_element(const multiply_2x2<N>&) {
    return {N(1), N(0), N(0), N(1)};
}

template <typename R, typename N>
R fibonacci(N n) {
    if (n == 0) return R(0);
    return power(std::array<R, 4>{1, 1, 1, 0}, N(n - 1), multiply_2x2<R>())[0];
}

} // namespace

BOOST_AUTO_TEST_CASE(all_low_prio_tasks_are_executed) {
    BOOST_TEST_MESSAGE("All low priority tasks are executed");

    serial_queue_t queue(low_executor);
    mutex m;
    vector<int> results;
    atomic_bool done{false};

    for (auto i = 0; i < 10; ++i) {
        queue.executor()([_i = i, &m, &results] {
            unique_lock<mutex> block{m};
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
            unique_lock<mutex> block{m};
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
            unique_lock<mutex> block{m};
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

    default_executor([&] {
        rest();
        {
            unique_lock<mutex> block{m};
            done = true;
        }
        cv.notify_one();
    });

    {
        unique_lock<mutex> block{m};
        while (!done) {
            cv.wait(block);
        }
    }

    default_executor([&] {
        rest();
        {
            unique_lock<mutex> block{m};
            done = false;
            cv.notify_one();
        }
    });

    {
        unique_lock<mutex> block{m};
        while (done) {
            cv.wait(block);
        }
    }

    BOOST_REQUIRE(!done);
}

namespace {
auto fiboN{1000};
const auto iterations = 100'000;
const auto startCount = 100;
atomic_int workToDo{(iterations - startCount) * 3};
const auto expectedWork = startCount * 3 + workToDo;
atomic_int highCount{0};
atomic_int defaultCount{0};
atomic_int lowCount{0};

atomic_int taskRunning{0};
atomic_int done{0};

atomic_int correctLow{0};
atomic_int correctDefault{0};
atomic_int correctHigh{0};

template <stlab::detail::executor_priority P>
struct check_task {
    atomic_int& _correctScheduleCount;
    atomic_int& _currentPrioCount;

    check_task(atomic_int& correctCount, atomic_int& prioCount) :
        _correctScheduleCount(correctCount), _currentPrioCount(prioCount) {
        ++_currentPrioCount;
    }

    template <stlab::detail::executor_priority Q>
    auto schedule_increment() -> std::enable_if_t<Q == stlab::detail::executor_priority::low, int> {
        return static_cast<int>(highCount <= taskRunning && defaultCount <= taskRunning);
    }

    template <stlab::detail::executor_priority Q>
    auto schedule_increment()
        -> std::enable_if_t<Q == stlab::detail::executor_priority::medium, int> {
        return static_cast<int>(highCount <= taskRunning);
    }

    template <stlab::detail::executor_priority Q>
    auto schedule_increment()
        -> std::enable_if_t<Q == stlab::detail::executor_priority::high, int> {
        return 0;
    }

    void operator()() {
        --_currentPrioCount;

        ++taskRunning;

        _correctScheduleCount += schedule_increment<P>();

        fibonacci<mpre::cpp_int>(fiboN);

        switch (workToDo % 3) {
            case 0:
                default_executor(check_task<stlab::detail::executor_priority::medium>{
                    correctDefault, defaultCount});
                break;

            case 1:
                high_executor(
                    check_task<stlab::detail::executor_priority::high>{correctHigh, highCount});
                break;

            case 2:
                low_executor(
                    check_task<stlab::detail::executor_priority::low>{correctLow, lowCount});
                break;
        }
        --workToDo;

        ++done;
        --taskRunning;
    }
};
} // namespace

BOOST_AUTO_TEST_CASE(all_tasks_will_be_executed_according_to_their_prio) {
    BOOST_TEST_MESSAGE("All tasks will be executed according to their prio");

    auto start = chrono::high_resolution_clock::now();

    for (auto i = 0; i < startCount; ++i) {
        low_executor(check_task<stlab::detail::executor_priority::low>{correctLow, lowCount});
        high_executor(check_task<stlab::detail::executor_priority::high>{correctHigh, highCount});
        default_executor(
            check_task<stlab::detail::executor_priority::medium>{correctDefault, defaultCount});
    }
    while (done < expectedWork) {
        rest();
    }

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout << "\nPerformance measuring: " << std::chrono::duration<double>(stop - start).count()
              << "s\n";

    // REVISIT (sean-parent) - I don't believe that this is measuring the probability of a task executing in order.stl
    cout << "Correct low ordering:     "
         << static_cast<double>(correctLow.load()) / iterations * 100.0 << "%\n";
    cout << "Correct default ordering: "
         << static_cast<double>(correctDefault.load()) / iterations * 100.0 << "%\n";
    cout << "Correct high ordering: "
         << static_cast<double>(correctHigh.load()) / iterations * 100.0 << "%\n";
}

BOOST_AUTO_TEST_CASE(MeasureTiming) {
    std::vector<int> results;
    const auto iterations = 10'000;
    results.resize(iterations * 3);
    atomic_bool done{false};
    condition_variable ready;
    atomic_int counter{0};

    auto start = chrono::high_resolution_clock::now();

    for (auto i = 0; i < iterations; ++i) {
        low_executor([_i = i, &results, &counter] {
            results[_i] = 1;
            fibonacci<mpre::cpp_int>(fiboN);
            ++counter;
        });
        default_executor([_i = i + iterations, &results, &counter] {
            results[_i] = 2;
            fibonacci<mpre::cpp_int>(fiboN);
            ++counter;
        });
        high_executor([_i = i + iterations * 2, &results, &counter] {
            results[_i] = 3;
            fibonacci<mpre::cpp_int>(fiboN);
            ++counter;
        });
    }

    mutex block;
    low_executor([&] {
        {
            unique_lock<mutex> lock{block};
            done = true;

            ready.notify_one();
        }
    });

    unique_lock<mutex> lock{block};
    while (!done)
        ready.wait(lock);

    while (counter < 3 * iterations) {
        rest();
    }

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout << "\nPerformance measuring: " << std::chrono::duration<double>(stop - start).count()
              << "s\n";

}
