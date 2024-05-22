/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#include <boost/test/unit_test.hpp>

#include <stlab/concurrency/future.hpp>
#include <stlab/concurrency/immediate_executor.hpp>
#include <stlab/concurrency/ready_future.hpp>

using namespace stlab;

BOOST_AUTO_TEST_SUITE(reduction_tests)

BOOST_AUTO_TEST_CASE(void_void_reduction) {
    auto f = make_ready_future(immediate_executor) |
             [] { return make_ready_future(immediate_executor); };
    BOOST_REQUIRE(!f.exception());
}

BOOST_AUTO_TEST_SUITE_END()
