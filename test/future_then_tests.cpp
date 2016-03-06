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

BOOST_FIXTURE_TEST_SUITE(future_void, success_fixture<void>)

    BOOST_AUTO_TEST_CASE(future_void_single_task) {
        int p = 0;

        sut = async(custom_scheduler<0>(), [&p] { p = 42; });
        check_valid_future(sut);

        wait_until_future_completed(sut);
    
        BOOST_REQUIRE_EQUAL(42, p);
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_void_two_tasks_with_same_scheduler) {
        std::atomic_int p = 0;

        sut = async(custom_scheduler<0>(), [_p = &p] { *_p = 42; })
            .then([_p = &p] { *_p += 42; });
        check_valid_future(sut);

        wait_until_future_completed(sut);

        BOOST_REQUIRE_EQUAL(42 + 42, p);
        BOOST_REQUIRE_EQUAL(2, custom_scheduler<0>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_int_void_two_tasks_with_same_scheduler) {
        std::atomic_int p = 0;

        sut = async(custom_scheduler<0>(), [_p = &p] { return 42; })
            .then([_p = &p](auto x) { *_p = x + 42; });
        check_valid_future(sut);

        wait_until_future_completed(sut);

        BOOST_REQUIRE_EQUAL(42 + 42, p);
        BOOST_REQUIRE_EQUAL(2, custom_scheduler<0>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_int_void_two_tasks_with_different_scheduler) {
        std::atomic_int p = 0;

        sut = async(custom_scheduler<0>(), [_p = &p] { return 42; })
            .then(custom_scheduler<1>(), [_p = &p](auto x) { *_p = x + 42; });
        check_valid_future(sut);

        wait_until_future_completed(sut);

        BOOST_REQUIRE_EQUAL(42 + 42, p);
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<1>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_void_two_tasks_with_different_scheduler) {
        std::atomic_int p = 0;

        sut = async(custom_scheduler<0>(), [_p = &p] { *_p = 42; })
            .then(custom_scheduler<1>(), [_p = &p] { *_p += 42; });
        check_valid_future(sut);

        wait_until_future_completed(sut);

        BOOST_REQUIRE_EQUAL(42 + 42, p);
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<1>::usage_counter());
    }

    /*
            f1
           /
        sut 
           \
            f2
    */
    BOOST_AUTO_TEST_CASE(future_void_Y_formation_tasks_with_same_scheduler) {
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
        BOOST_REQUIRE_EQUAL(3, custom_scheduler<0>::usage_counter());
    }
BOOST_AUTO_TEST_SUITE_END()



BOOST_FIXTURE_TEST_SUITE(future_int, success_fixture<int>)

    BOOST_AUTO_TEST_CASE(future_int_single_task) {
        sut = async(custom_scheduler<0>(), [] { return 42; });
        check_valid_future(sut);

        wait_until_future_completed(sut);

        BOOST_REQUIRE_EQUAL(42, *sut.get_try());
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_int_two_tasks_with_same_scheduler) {
        sut = async(custom_scheduler<0>(), [] { return 42; })
            .then([](auto x) { return x + 42; });
        check_valid_future(sut);

        wait_until_future_completed(sut);

        BOOST_REQUIRE_EQUAL(42 + 42, *sut.get_try());
        BOOST_REQUIRE_EQUAL(2, custom_scheduler<0>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_int_two_tasks_with_different_scheduler) {
        auto sut = async(custom_scheduler<0>(), [] { return 42; })
            .then(custom_scheduler<1>(), [](auto x) { return x + 42; });
        check_valid_future(sut);

        wait_until_future_completed(sut);

        BOOST_REQUIRE_EQUAL(42 + 42, *sut.get_try());
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<1>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_void_int_two_tasks_with_same_scheduler) {
        std::atomic_int p = 0;

        sut = async(custom_scheduler<0>(), [_p = &p] { *_p = 42; })
            .then([_p = &p] { *_p += 42; return _p->load(); });
        check_valid_future(sut);

        wait_until_future_completed(sut);

        BOOST_REQUIRE_EQUAL(42 + 42, p);
        BOOST_REQUIRE_EQUAL(2, custom_scheduler<0>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_void_int_two_tasks_with_different_scheduler) {
        std::atomic_int p = 0;

        sut = async(custom_scheduler<0>(), [_p = &p] { *_p = 42; })
            .then(custom_scheduler<1>(), [_p = &p] { *_p += 42; return _p->load(); });
        check_valid_future(sut);

        wait_until_future_completed(sut);

        BOOST_REQUIRE_EQUAL(42 + 42, p);
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<1>::usage_counter());
    }


    /*
        sut - f - f
    */
    BOOST_AUTO_TEST_CASE(future_int_three_tasks_with_same_scheduler) {
        auto sut = async(custom_scheduler<0>(), [] { return 42; })
            .then(custom_scheduler<0>(), [](auto x) { return x + 42; })
            .then(custom_scheduler<0>(), [](auto x) { return x + 42; });
        check_valid_future(sut);

        wait_until_future_completed(sut);

        BOOST_REQUIRE_EQUAL(42 + 42 + 42, *sut.get_try());
        BOOST_REQUIRE_EQUAL(3, custom_scheduler<0>::usage_counter());
    }

    /*
            f1
           /
        sut 
           \
            f2
    */
    BOOST_AUTO_TEST_CASE(future_int_Y_formation_tasks_with_same_scheduler) {
        auto sut = async(custom_scheduler<0>(), [] { return 42; });
        auto f1 = sut.then(custom_scheduler<0>(), [](auto x) -> int { return x + 42; });
        auto f2 = sut.then(custom_scheduler<0>(), [](auto x) -> int { return x + 4177; });
        check_valid_future(sut, f1, f2);

        wait_until_future_completed(f1, f2);

        BOOST_REQUIRE_EQUAL(42 + 42, *f1.get_try());
        BOOST_REQUIRE_EQUAL(42 + 4177, *f2.get_try());
        BOOST_REQUIRE_EQUAL(3, custom_scheduler<0>::usage_counter());
    }
 
BOOST_AUTO_TEST_SUITE_END()

// ----------------------------------------------------------------------------
//                             Error cases
// ----------------------------------------------------------------------------
#if 0
    BOOST_AUTO_TEST_CASE(future_void_single_task_error) {
        test_setup setup;

        auto sut = async(custom_scheduler<0>(), [] { throw test_exception("falure"); });
        BOOST_REQUIRE(sut.valid() == true);
        BOOST_REQUIRE(sut.error().is_initialized() == false);

        while (!sut.get_try());

        BOOST_REQUIRE_EQUAL(42, p);
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_void_two_tasks_with_same_scheduler) {
        test_setup setup;
        std::atomic_int p = 0;

        auto sut = async(custom_scheduler<0>(), [_p = &p] { *_p = 42; }).then([_p = &p] { *_p += 42; });
        BOOST_REQUIRE(sut.valid() == true);
        BOOST_REQUIRE(sut.error().is_initialized() == false);

        while (!sut.get_try());

        BOOST_REQUIRE_EQUAL(42 + 42, p);
        BOOST_REQUIRE_EQUAL(2, custom_scheduler<0>::usage_counter());
    }
#endif