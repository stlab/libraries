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

auto create_void_future() {
    return future<void>();
}

auto create_non_void_future() {
    return future<int>();
}

auto create_movable_only_future() {
    return future<std::unique_ptr<int>>();
}

BOOST_AUTO_TEST_CASE(future_default_constructed_void) {
    auto sut = create_void_future();
    BOOST_REQUIRE(sut.valid() == false);
    BOOST_REQUIRE(sut.cancel_try() == false);
}

BOOST_AUTO_TEST_CASE(future_default_constructed_non_void) {
    auto sut = create_non_void_future();
    BOOST_REQUIRE(sut.valid() == false);
    BOOST_REQUIRE(sut.cancel_try() == false);
}

BOOST_AUTO_TEST_CASE(future_default_constructed_move_only) {
    auto sut = create_movable_only_future();
    BOOST_REQUIRE(sut.valid() == false);
    BOOST_REQUIRE(sut.cancel_try() == false);
}
