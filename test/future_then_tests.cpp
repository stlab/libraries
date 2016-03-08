/*
Copyright 2016 Felix Petriconi
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <stlab/future.hpp>

#include "test_helper.hpp"

using namespace stlab;
using namespace test_helper;

BOOST_FIXTURE_TEST_SUITE(future_then_void, success_fixture<void>)

    BOOST_AUTO_TEST_CASE(future_void_single_task) {
        BOOST_TEST_MESSAGE("running future void single task");

        int p = 0;

        sut = async(custom_scheduler<0>(), [&p] { p = 42; });
        check_valid_future(sut);

        wait_until_future_completed(sut);
    
        BOOST_REQUIRE_EQUAL(42, p);
        BOOST_REQUIRE_LE(1, custom_scheduler<0>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_void_two_tasks_with_same_scheduler) {
        BOOST_TEST_MESSAGE("running future void with two task on same scheduler");

        std::atomic_int p = 0;

        sut = async(custom_scheduler<0>(), [_p = &p] { *_p = 42; })
            .then([_p = &p] { *_p += 42; });
        check_valid_future(sut);

        wait_until_future_completed(sut);

        BOOST_REQUIRE_EQUAL(42 + 42, p);
        BOOST_REQUIRE_LE(2, custom_scheduler<0>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_int_void_two_tasks_with_same_scheduler) {
        BOOST_TEST_MESSAGE("running future int void tasks with same scheduler");

        std::atomic_int p = 0;

        sut = async(custom_scheduler<0>(), [_p = &p] { return 42; })
            .then([_p = &p](auto x) { *_p = x + 42; });
        check_valid_future(sut);

        wait_until_future_completed(sut);

        BOOST_REQUIRE_EQUAL(42 + 42, p);
        BOOST_REQUIRE_LE(2, custom_scheduler<0>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_int_void_two_tasks_with_different_scheduler) {
        BOOST_TEST_MESSAGE("running future int void tasks with different schedulers");

        std::atomic_int p = 0;

        sut = async(custom_scheduler<0>(), [_p = &p] { return 42; })
            .then(custom_scheduler<1>(), [_p = &p](auto x) { *_p = x + 42; });
        check_valid_future(sut);

        wait_until_future_completed(sut);

        BOOST_REQUIRE_EQUAL(42 + 42, p);
        BOOST_REQUIRE_LE(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_void_two_tasks_with_different_scheduler) {
        BOOST_TEST_MESSAGE("running future void two tasks with different schedulers");

        std::atomic_int p = 0;

        sut = async(custom_scheduler<0>(), [_p = &p] { *_p = 42; })
            .then(custom_scheduler<1>(), [_p = &p] { *_p += 42; });
        check_valid_future(sut);

        wait_until_future_completed(sut);

        BOOST_REQUIRE_EQUAL(42 + 42, p);
        BOOST_REQUIRE_LE(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
    }

    /*
            f1
           /
        sut 
           \
            f2
    */
    BOOST_AUTO_TEST_CASE(future_void_Y_formation_tasks_with_same_scheduler) {
        BOOST_TEST_MESSAGE("running future void with Y formation with same scheduler");

        std::atomic_int p = 0;
        int r1 = 0;
        int r2 = 0;

        auto sut = async(custom_scheduler<0>(), [_p = &p ] { *_p = 42; });
        auto f1 = sut.then(custom_scheduler<0>(), [_p = &p, _r = &r1 ] { *_r = 42 + *_p; });
        auto f2 = sut.then(custom_scheduler<0>(), [_p = &p, _r = &r2 ] { *_r = 4711 + *_p; });
        check_valid_future(sut, f1, f2);

        wait_until_future_completed(f1, f2);

        BOOST_REQUIRE_EQUAL(42 + 42, r1);
        BOOST_REQUIRE_EQUAL(42 + 4711, r2);
        BOOST_REQUIRE_LE(3, custom_scheduler<0>::usage_counter());
    }
BOOST_AUTO_TEST_SUITE_END()



BOOST_FIXTURE_TEST_SUITE(future_then_int, success_fixture<int>)

    BOOST_AUTO_TEST_CASE(future_int_single_task) {
        BOOST_TEST_MESSAGE("running future int single tasks");

        sut = async(custom_scheduler<0>(), [] { return 42; });
        check_valid_future(sut);

        wait_until_future_completed(sut);

        BOOST_REQUIRE_EQUAL(42, *sut.get_try());
        BOOST_REQUIRE_LE(1, custom_scheduler<0>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_int_two_tasks_with_same_scheduler) {
        BOOST_TEST_MESSAGE("running future int two tasks with same scheduler");

        sut = async(custom_scheduler<0>(), [] { return 42; })
            .then([](auto x) { return x + 42; });
        check_valid_future(sut);

        wait_until_future_completed(sut);

        BOOST_REQUIRE_EQUAL(42 + 42, *sut.get_try());
        BOOST_REQUIRE_LE(2, custom_scheduler<0>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_int_two_tasks_with_different_scheduler) {
        BOOST_TEST_MESSAGE("running future int two tasks with different scheduler");

        auto sut = async(custom_scheduler<0>(), [] { return 42; })
            .then(custom_scheduler<1>(), [](auto x) { return x + 42; });
        check_valid_future(sut);

        wait_until_future_completed(sut);

        BOOST_REQUIRE_EQUAL(42 + 42, *sut.get_try());
        BOOST_REQUIRE_LE(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_void_int_two_tasks_with_same_scheduler) {
        BOOST_TEST_MESSAGE("running future void int tasks with same scheduler");

        std::atomic_int p = 0;

        sut = async(custom_scheduler<0>(), [_p = &p] { *_p = 42; })
            .then([_p = &p] { *_p += 42; return _p->load(); });
        check_valid_future(sut);

        wait_until_future_completed(sut);

        BOOST_REQUIRE_EQUAL(42 + 42, p);
        BOOST_REQUIRE_LE(2, custom_scheduler<0>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_void_int_two_tasks_with_different_scheduler) {
        BOOST_TEST_MESSAGE("running future void int tasks with different schedulers");
        std::atomic_int p = 0;

        sut = async(custom_scheduler<0>(), [_p = &p] { *_p = 42; })
            .then(custom_scheduler<1>(), [_p = &p] { *_p += 42; return _p->load(); });
        check_valid_future(sut);

        wait_until_future_completed(sut);

        BOOST_REQUIRE_EQUAL(42 + 42, p);
        BOOST_REQUIRE_LE(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
    }


    /*
        sut - f - f
    */
    BOOST_AUTO_TEST_CASE(future_int_three_tasks_with_same_scheduler) {
        BOOST_TEST_MESSAGE("running future int with three tasks with same scheduler");

        auto sut = async(custom_scheduler<0>(), [] { return 42; })
            .then(custom_scheduler<0>(), [](auto x) { return x + 42; })
            .then(custom_scheduler<0>(), [](auto x) { return x + 42; });
        check_valid_future(sut);

        wait_until_future_completed(sut);

        BOOST_REQUIRE_EQUAL(42 + 42 + 42, *sut.get_try());
        BOOST_REQUIRE_LE(3, custom_scheduler<0>::usage_counter());
    }

    /*
            f1
           /
        sut 
           \
            f2
    */
    BOOST_AUTO_TEST_CASE(future_int_Y_formation_tasks_with_same_scheduler) {
        BOOST_TEST_MESSAGE("running future int Y formation tasks with same scheduler");

        sut = async(custom_scheduler<0>(), [] { return 42; });
        auto f1 = sut.then(custom_scheduler<0>(), [](auto x) -> int { return x + 42; });
        auto f2 = sut.then(custom_scheduler<0>(), [](auto x) -> int { return x + 4177; });
        check_valid_future(sut, f1, f2);

        wait_until_future_completed(f1, f2);

        BOOST_REQUIRE_EQUAL(42 + 42, *f1.get_try());
        BOOST_REQUIRE_EQUAL(42 + 4177, *f2.get_try());
        BOOST_REQUIRE_LE(3, custom_scheduler<0>::usage_counter());
    }
 BOOST_AUTO_TEST_SUITE_END()

// ----------------------------------------------------------------------------
//                             Error cases
// ----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_SUITE(future_void_then_error, failure_fixture<void>)
    BOOST_AUTO_TEST_CASE(future_void_single_task_error) {
        BOOST_TEST_MESSAGE("running future void with single tasks that fails");

        sut = async(custom_scheduler<0>(), [] { throw test_exception("failure"); });

        wait_until_future_fails<test_exception>(sut);

        check_failure<test_exception>(sut, std::string("failure"));
        BOOST_REQUIRE_LE(1, custom_scheduler<0>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_void_two_tasks_error_in_1st_task_with_same_scheduler) {
        BOOST_TEST_MESSAGE("running future void with two tasks which first fails");
        std::atomic_int p = 0;

        auto sut = async(custom_scheduler<0>(), [_p = &p] { throw test_exception("failure"); })
            .then([_p = &p] { *_p = 42; });

        wait_until_future_fails<test_exception>(sut);

        check_failure<test_exception>(sut, std::string("failure"));
        BOOST_REQUIRE_EQUAL(0, p);
        BOOST_REQUIRE_LE(1, custom_scheduler<0>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_void_two_tasks_error_in_2nd_task_with_same_scheduler) {
        BOOST_TEST_MESSAGE("running future void with two tasks which second fails");
        std::atomic_int p = 0;

        auto sut = async(custom_scheduler<0>(), [_p = &p] { *_p = 42; })
            .then([_p = &p] { throw test_exception("failure"); });

        wait_until_future_fails<test_exception>(sut);

        check_failure<test_exception>(sut, std::string("failure"));
        BOOST_REQUIRE_EQUAL(42, p);
        BOOST_REQUIRE_LE(2, custom_scheduler<0>::usage_counter());
    }
BOOST_AUTO_TEST_SUITE_END()


BOOST_FIXTURE_TEST_SUITE(future_then_int_error, failure_fixture<int>)

    BOOST_AUTO_TEST_CASE(future_int_single_task_error) {
        BOOST_TEST_MESSAGE("running future int with single tasks that fails");

        sut = async(custom_scheduler<0>(), []()->int { throw test_exception("failure"); });
        wait_until_future_fails<test_exception>(sut);

        check_failure<test_exception>(sut, std::string("failure"));
        BOOST_REQUIRE_LE(1, custom_scheduler<0>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_int_two_tasks_error_in_1st_task_with_same_scheduler) {
        BOOST_TEST_MESSAGE("running future int with two tasks which first fails");
        int p = 0;

        auto sut = async(custom_scheduler<0>(), [] { throw test_exception("failure"); })
            .then([_p = &p]()->int { *_p = 42; return *_p; });

        wait_until_future_fails<test_exception>(sut);

        check_failure<test_exception>(sut, std::string("failure"));
        BOOST_REQUIRE_EQUAL(0, p);
        BOOST_REQUIRE_LE(1, custom_scheduler<0>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_int_two_tasks_error_in_2nd_task_with_same_scheduler) {
        BOOST_TEST_MESSAGE("running future void with two tasks which second fails");
        std::atomic_int p = 0;

        auto sut = async(custom_scheduler<0>(), [_p = &p] { *_p = 42; })
            .then([]()->int { throw test_exception("failure"); });

        wait_until_future_fails<test_exception>(sut);

        check_failure<test_exception>(sut, std::string("failure"));
        BOOST_REQUIRE_EQUAL(42, p);
        BOOST_REQUIRE_LE(2, custom_scheduler<0>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_int_Y_formation_tasks_with_failing_1st_task) {
        BOOST_TEST_MESSAGE("running future int Y formation tasks where the 1st tasks fails");
        std::atomic_int p = 0;

        sut = async(custom_scheduler<0>(), []()->int { throw test_exception("failure"); });
        auto f1 = sut.then(custom_scheduler<0>(), [_p = &p](auto x) -> int { *_p += 1; return x + 42; });
        auto f2 = sut.then(custom_scheduler<0>(), [_p = &p](auto x) -> int { *_p += 1;  return x + 4177; });

        wait_until_future_fails<test_exception>(f1, f2);

        check_failure<test_exception>(f1, std::string("failure"));
        check_failure<test_exception>(f2, std::string("failure"));
        BOOST_REQUIRE_EQUAL(0, p);
        BOOST_REQUIRE_LE(1, custom_scheduler<0>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_int_Y_formation_tasks_where_one_of_the_2nd_task_failing) {
        BOOST_TEST_MESSAGE("running future int Y formation tasks where one of the 2nd tasks fails");

        sut = async(custom_scheduler<0>(), []()->int { return 42; });
        auto f1 = sut.then(custom_scheduler<0>(), [](auto x) -> int { throw test_exception("failure"); });
        auto f2 = sut.then(custom_scheduler<0>(), [](auto x) -> int { return x + 4711; });

        wait_until_future_completed(f2);
        wait_until_future_fails<test_exception>(f1);

        check_failure<test_exception>(f1, std::string("failure"));
        BOOST_REQUIRE_EQUAL(42 + 4711, *f2.get_try());
        BOOST_REQUIRE_LE(3, custom_scheduler<0>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_int_Y_formation_tasks_where_both_of_the_2nd_task_failing) {
        BOOST_TEST_MESSAGE("running future int Y formation tasks where both of the 2nd tasks fails");

        sut = async(custom_scheduler<0>(), []()->int { return 42; });
        auto f1 = sut.then(custom_scheduler<0>(), [](auto x) -> int { throw test_exception("failure"); });
        auto f2 = sut.then(custom_scheduler<0>(), [](auto x) -> int { throw test_exception("failure"); });

        wait_until_future_fails<test_exception>(f1, f2);

        check_failure<test_exception>(f1, std::string("failure"));
        check_failure<test_exception>(f2, std::string("failure"));
        BOOST_REQUIRE_LE(3, custom_scheduler<0>::usage_counter());
    }

BOOST_AUTO_TEST_SUITE_END()