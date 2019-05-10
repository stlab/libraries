/*
Copyright 2019 Adobe
    Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#include <boost/test/unit_test.hpp>

#include <stlab/concurrency/traits.hpp>
#include <stlab/test/model.hpp>

#include <vector>

using namespace stlab;

namespace stlab
{

template<template<typename> class test, typename T, typename A>
struct smart_test<test, std::vector<T, A>> : test<T> {};

template<template<typename> class test, typename T, typename A>
struct smart_test<test, std::vector<std::vector<T, A>>> : test<T> {};

}

BOOST_AUTO_TEST_CASE(check_smart_is_copy_constructible_v) {
    BOOST_TEST_MESSAGE("Check smart_is_copy_constructible_v");

    BOOST_REQUIRE(smart_is_copy_constructible_v<int> == true);
    BOOST_REQUIRE(smart_is_copy_constructible_v<std::vector<int>> == true);
    BOOST_REQUIRE(smart_is_copy_constructible_v<std::vector<annotate>> == true);

    BOOST_REQUIRE(smart_is_copy_constructible_v<move_only> == false);
    BOOST_REQUIRE(smart_is_copy_constructible_v<std::vector<move_only>> == false);
    BOOST_REQUIRE(smart_is_copy_constructible_v<std::vector<std::vector<move_only>>> == false);
}