/*
Copyright 2015 Adobe
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <boost/mpl/list.hpp>

#include <stlab/future.hpp>

using namespace stlab;

using all_test_types = boost::mpl::list<void, int, std::unique_ptr<int>>;

BOOST_AUTO_TEST_CASE_TEMPLATE(future_default_constructed, T, all_test_types)
{
    auto sut = future<T>();
    BOOST_REQUIRE(sut.valid() == false);
    BOOST_REQUIRE(sut.cancel_try() == false);
}

using copyable_test_types = boost::mpl::list<void, int>;

BOOST_AUTO_TEST_CASE_TEMPLATE(future_constructed_minimal_fn, T, copyable_test_types) {
    auto sut = async(default_scheduler(), std::function<T()>());
    BOOST_REQUIRE(sut.valid() == true);
    BOOST_REQUIRE(sut.error().is_initialized() == false);

    BOOST_REQUIRE(sut.cancel_try() == false);
    BOOST_REQUIRE(sut.valid() == true);
}

#if 0
BOOST_AUTO_TEST_CASE(future_constructed_minimal_fn_moveonly) {
    auto sut = async(default_scheduler(), []()->std::unique_ptr<int> { 
        auto r = std::make_unique<int>(); 
        *r = 42; 
        return std::move(r); 
    });
    BOOST_REQUIRE(sut.valid() == true);
    BOOST_REQUIRE(sut.error().is_initialized() == false);

    while (!sut.get_try()) {}
    auto result = sut.get_try();
    
    BOOST_REQUIRE(**result == 42);
    BOOST_REQUIRE(sut.cancel_try() == false);
    BOOST_REQUIRE(sut.valid() == true);
}

#endif