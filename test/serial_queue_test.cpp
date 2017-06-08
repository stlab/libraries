/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#include <stlab/default_executor.hpp>
#include <stlab/serial_queue.hpp>
#include <thread>

#include <boost/test/unit_test.hpp>
#include "test_helper.hpp"

using namespace stlab;

BOOST_AUTO_TEST_CASE(SerialQueue_DefaultConstruction) {

    serial_queue sut;
}

BOOST_AUTO_TEST_CASE(SerialQueue_EnqueueOneTask) {

    serial_queue sut;
    std::atomic_bool flag{false};

    sut.executor()([&_flag = flag]{ _flag = true; });

    while (!flag) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    BOOST_REQUIRE_EQUAL(true, flag);
}

BOOST_AUTO_TEST_CASE(SerialQueue_EnqueueTwoTasks) {

    serial_queue sut;

    std::atomic_bool flagA{false};
    std::atomic_bool flagB{false};
    std::vector<int> results;

    sut.executor()([&_flag = flagA, &_results = results]{ _results.push_back(0); _flag = true; });
    sut.executor()([&_flag = flagB, &_results = results]{ _results.push_back(1); _flag = true; });

    while (!flagA && ! flagB) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    BOOST_REQUIRE_EQUAL(2, results.size());
    BOOST_REQUIRE_EQUAL(0, results[0]);
    BOOST_REQUIRE_EQUAL(1, results[1]);
}

BOOST_AUTO_TEST_CASE(SerialQueue_EnqueueManyTasks) {

    serial_queue sut;

    std::atomic_int counter{0};
    std::vector<int> results;

    for (auto i = 0; i < 100; ++i) {
        sut.executor()([&_counter = counter, &_results = results, i]{ _results.push_back(i); ++_counter; });
    }

    while (counter < 100) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    BOOST_REQUIRE_EQUAL(100, results.size());
    for (auto i = 0; i < 100; ++i) {
        BOOST_REQUIRE_EQUAL(i, results[i]);
    }
}

BOOST_AUTO_TEST_CASE(SerialQueue_EnqueueManyTasksInDifferentQueues) {

    serial_queue sutA;
    serial_queue sutB;

    std::atomic_int counter{0};
    std::vector<int> resultsA, resultsB;

    for (auto i = 0; i < 100; ++i) {
        sutA.executor()([&_counter = counter, &_results = resultsA, i]{ _results.push_back(i); ++_counter; });
        sutB.executor()([&_counter = counter, &_results = resultsB, i]{ _results.push_back(i+100); ++_counter; });
    }

    while (counter < 200) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    BOOST_REQUIRE_EQUAL(100, resultsA.size());
    BOOST_REQUIRE_EQUAL(100, resultsB.size());
    for (auto i = 0; i < 100; ++i) {
        BOOST_REQUIRE_EQUAL(i, resultsA[i]);
        BOOST_REQUIRE_EQUAL(i+100, resultsB[i]);
    }
}


BOOST_AUTO_TEST_CASE(SerialQueue_CopyConstruction_EnsureCopyUsesSourceQueue) {

    serial_queue sutA(test_help::no_executor{});
    serial_queue sutB(sutA);

    sutA.execute()([](){});

    BOOST_REQUIRE_EQUAL(1, test_help::no_executor::tasks.size());
}

