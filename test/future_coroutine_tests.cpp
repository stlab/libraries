/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#include <boost/test/unit_test.hpp>

#include <stlab/concurrency/future.hpp>
#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/utility.hpp>

#include <stlab/test/model.hpp>

#include <thread>

#ifdef STLAB_FUTURE_COROUTINE_SUPPORT

using namespace stlab;

stlab::future<int> get_the_answer() {
    auto result = co_await stlab::async(stlab::default_executor,
                                        [] {
                                            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                                            return 42;
                                        });

    co_return result;
}

BOOST_AUTO_TEST_CASE(future_coroutine_int) {
    BOOST_TEST_MESSAGE("future coroutine int");
    auto w = get_the_answer();

    BOOST_REQUIRE(42 == blocking_get(w));
}


stlab::future<move_only> get_move_only_answer() {
    auto result = co_await stlab::async(stlab::default_executor,
                                        [] {
                                            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                                            return move_only{42};
                                        });

    co_return std::move(result);
}

BOOST_AUTO_TEST_CASE(future_coroutine_move_only) {
    BOOST_TEST_MESSAGE("future coroutine move only");
    auto w = get_move_only_answer();
    auto r = blocking_get(std::move(w));

    BOOST_REQUIRE(42 == r.member());
}


stlab::future<void> just_wait() {
    co_await stlab::async(stlab::default_executor,
    [] {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    });
}


BOOST_AUTO_TEST_CASE(future_coroutine_void) {
    BOOST_TEST_MESSAGE("future coroutine void");

    auto w = just_wait();

    BOOST_REQUIRE_NO_THROW(blocking_get(w));
}

#endif