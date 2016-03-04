/*
Copyright 2015 Adobe
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <stlab/future.hpp>

using namespace stlab;

namespace
{
    struct custom_scheduler {
        using result_type = void;

        template <typename F>
        void operator()(F f) {
            ++_usage_counter;
            detail::async_(std::move(f));
        }

        static int usage_counter() { return _usage_counter; }

        static void reset_usage_counter() { _usage_counter = 0; }

    private:
        static int _usage_counter;
    };

    int custom_scheduler::_usage_counter = 0;

    struct test_setup
    {
        test_setup() {
            custom_scheduler::reset_usage_counter();
        }
    };
}

BOOST_AUTO_TEST_CASE(future_void_single_task) {
    test_setup setup;
    int p = 0;

    auto sut = async(custom_scheduler(), [&p] { p = 42; });
    BOOST_REQUIRE(sut.valid() == true);
    BOOST_REQUIRE(sut.error().is_initialized() == false);

    while (!sut.get_try());
    
    BOOST_REQUIRE_EQUAL(42, p);
    BOOST_REQUIRE_EQUAL(1, custom_scheduler::usage_counter());
}

BOOST_AUTO_TEST_CASE(future_void_two_tasks) {
    test_setup setup;
    std::atomic_int p = 0;

    auto sut = async(custom_scheduler(), [_p = &p] { *_p = 42; }).then([_p = &p]{ *_p += 42; });
    BOOST_REQUIRE(sut.valid() == true);
    BOOST_REQUIRE(sut.error().is_initialized() == false);

    while (!sut.get_try());
    
    BOOST_REQUIRE_EQUAL(42 + 42, p);
    BOOST_REQUIRE_EQUAL(2, custom_scheduler::usage_counter());
}


BOOST_AUTO_TEST_CASE(future_int_single_task) {
    test_setup setup;

    auto sut = async(custom_scheduler(), [] { return 42; });
    BOOST_REQUIRE(sut.valid() == true);
    BOOST_REQUIRE(sut.error().is_initialized() == false);

    while (!sut.get_try());

    BOOST_REQUIRE_EQUAL(42, *sut.get_try());
    BOOST_REQUIRE_EQUAL(1, custom_scheduler::usage_counter());
}

BOOST_AUTO_TEST_CASE(future_int_two_tasks) {
    test_setup setup;

    auto sut = async(custom_scheduler(), [] { return 42; }).then([](auto x) { return x + 42; });
    BOOST_REQUIRE(sut.valid() == true);
    BOOST_REQUIRE(sut.error().is_initialized() == false);

    while (!sut.get_try());

    BOOST_REQUIRE_EQUAL(42 + 42, *sut.get_try());
    BOOST_REQUIRE_EQUAL(2, custom_scheduler::usage_counter());
}

BOOST_AUTO_TEST_CASE(future_int_void_two_tasks) {
    test_setup setup;
    std::atomic_int p = 0;

    auto sut = async(custom_scheduler(), [_p = &p] { return 42; }).then([_p = &p](auto x) { *_p = x + 42; });
    BOOST_REQUIRE(sut.valid() == true);
    BOOST_REQUIRE(sut.error().is_initialized() == false);

    while (!sut.get_try());

    BOOST_REQUIRE_EQUAL(42 + 42, p);
    BOOST_REQUIRE_EQUAL(2, custom_scheduler::usage_counter());
}

BOOST_AUTO_TEST_CASE(future_void_int_two_tasks) {
    test_setup setup;
    std::atomic_int p = 0;

    auto sut = async(custom_scheduler(), [_p = &p] { *_p = 42; }).then([_p = &p] { *_p += 42; return _p->load(); });
    BOOST_REQUIRE(sut.valid() == true);
    BOOST_REQUIRE(sut.error().is_initialized() == false);

    while (!sut.get_try());

    BOOST_REQUIRE_EQUAL(42 + 42, p);
    BOOST_REQUIRE_EQUAL(2, custom_scheduler::usage_counter());
}
