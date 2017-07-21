/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#include <boost/test/unit_test.hpp>
#include <boost/mpl/list.hpp>

#include <stlab/concurrency/concurrency.hpp>

#include "test_helper.hpp"
#include "future_test_helper.hpp"

using namespace stlab;
using namespace future_test_helper;

using all_test_types = boost::mpl::list<void, int, std::unique_ptr<int>>;

BOOST_AUTO_TEST_CASE_TEMPLATE(future_default_constructed, T, all_test_types)
{
    BOOST_TEST_MESSAGE("running future default constructed of type " << typeid(T).name());

    auto sut = future<T>();
    BOOST_REQUIRE(sut.valid() == false);
}

using copyable_test_types = boost::mpl::list<int, double>;

BOOST_AUTO_TEST_CASE_TEMPLATE(future_constructed_minimal_fn, T, copyable_test_types) {
    BOOST_TEST_MESSAGE("running future with minimal copyable type " << typeid(T).name());

    test_setup setup;
    {
        auto sut = async(custom_scheduler<0>(), []()->T { return T(0); });
        BOOST_REQUIRE(sut.valid() == true);
        BOOST_REQUIRE(sut.error().is_initialized() == false);

        sut.reset();
        BOOST_REQUIRE(sut.valid() == false);
    }
    BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(future_constructed_minimal_fn_with_parameters, T, copyable_test_types) {
    BOOST_TEST_MESSAGE("running future with minimal copyable type and passed parameter " << typeid(T).name());

    test_setup setup;
    {
        auto sut = async(custom_scheduler<0>(), [](auto x)->T { return x + T(0); }, T(42));
        BOOST_REQUIRE(sut.valid() == true);
        BOOST_REQUIRE(sut.error().is_initialized() == false);

        while (!sut.get_try()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        BOOST_WARN_EQUAL(T(42) + T(0), *sut.get_try());
    }
    BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
}

BOOST_AUTO_TEST_CASE(future_constructed_minimal_fn_moveonly) {
    BOOST_TEST_MESSAGE("running future with move only type");
    
    test_setup setup;
    {
        auto sut = async(custom_scheduler<0>(), []()->test_helper::move_only_t {
            return test_helper::move_only_t{ 42 };
        });
        BOOST_REQUIRE(sut.valid() == true);
        BOOST_REQUIRE(sut.error().is_initialized() == false);
        
        while (!sut.get_try()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        
        BOOST_REQUIRE_EQUAL(42, sut.get_try().get().member());
    }
    BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
}
