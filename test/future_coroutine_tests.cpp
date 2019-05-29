/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#include <boost/test/unit_test.hpp>

#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/future.hpp>
#include <stlab/concurrency/utility.hpp>

#include <stlab/test/model.hpp>

#include <thread>

#include "future_test_helper.hpp"

#if STLAB_FUTURE_COROUTINES

using namespace stlab;
using namespace future_test_helper;

stlab::future<int> get_the_answer() { co_return 42; }

BOOST_AUTO_TEST_CASE(future_coroutine_int) {
    BOOST_TEST_MESSAGE("future coroutine int");

    auto w = get_the_answer();

    BOOST_REQUIRE(42 == blocking_get(w));
}

stlab::future<move_only> get_move_only_answer() { co_return move_only{42}; }

BOOST_AUTO_TEST_CASE(future_coroutine_move_only) {
    BOOST_TEST_MESSAGE("future coroutine move only");

    auto w = get_move_only_answer();
    auto r = blocking_get(std::move(w));

    BOOST_REQUIRE(42 == r.member());
}

stlab::future<void> just_wait() { co_return; }

BOOST_AUTO_TEST_CASE(future_coroutine_void) {
    BOOST_TEST_MESSAGE("future coroutine void");

    auto w = just_wait();

    BOOST_REQUIRE_NO_THROW(blocking_get(w));
}

stlab::future<int> get_the_answer_with_failure() {
    auto result = co_await stlab::async(stlab::default_executor, [] {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        return 42;
    });
    throw test_exception("failure");

    co_return result;
}

BOOST_AUTO_TEST_CASE(future_coroutine_int_failure) {
    BOOST_TEST_MESSAGE("future coroutine int with failure");

    auto w = get_the_answer_with_failure();

    BOOST_REQUIRE_EXCEPTION(blocking_get(w), test_exception,
                            ([_m = std::string("failure")](const auto& e) {
                                return std::string(_m) == std::string(e.what());
                            }));
}

stlab::future<move_only> get_the_answer_move_only_with_failure() {
    auto result = co_await stlab::async(stlab::default_executor, [] {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        return move_only{42};
    });
    throw test_exception("failure");

    co_return std::move(result);
}

BOOST_AUTO_TEST_CASE(future_coroutine_move_only_failure) {
    BOOST_TEST_MESSAGE("future coroutine move only with failure");

    auto w = get_the_answer_move_only_with_failure();

    BOOST_REQUIRE_EXCEPTION(blocking_get(std::move(w)), test_exception,
                            ([_m = std::string("failure")](const auto& e) {
                                return std::string(_m) == std::string(e.what());
                            }));
}

future<void> do_it(future<int> x, std::atomic_int& result) {
    int v = co_await x;
    result = v;
    std::cout << v << '\n';
    co_return;
}

BOOST_AUTO_TEST_CASE(future_coroutine_combined_void_int) {
    BOOST_TEST_MESSAGE("future coroutine combination of void and int future");
    std::atomic_int intCheck{0};
    std::atomic_bool boolCheck{false};

    auto done = do_it(async(default_executor, [] { return 42; }), intCheck);
    auto hold = done.then([&boolCheck] { boolCheck = true; });

    blocking_get(hold);

    BOOST_REQUIRE_EQUAL(42, intCheck);
    BOOST_REQUIRE(boolCheck.load());
}

#endif
