/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#include <boost/test/unit_test.hpp>

#include <stlab/concurrency/tuple_algorithm.hpp>

using namespace stlab;

BOOST_AUTO_TEST_SUITE(tuple_find_test)

BOOST_AUTO_TEST_CASE(empty_tuple) {
    std::tuple<> t;
    BOOST_REQUIRE_EQUAL(std::size_t(1), stlab::tuple_find(t, [](const auto&) { return false; }));
}

BOOST_AUTO_TEST_CASE(one_element_tuple_that_fails) {
    std::tuple<std::vector<int>> t;
    BOOST_REQUIRE_EQUAL(std::size_t(1),
                        stlab::tuple_find(t, [](const auto& t) { return !t.empty(); }));
}

BOOST_AUTO_TEST_CASE(one_element_tuple_that_succeeds) {
    std::tuple<std::vector<int>> t;
    BOOST_REQUIRE_EQUAL(std::size_t(0),
                        stlab::tuple_find(t, [](const auto& t) { return t.empty(); }));
}

BOOST_AUTO_TEST_CASE(two_element_tuple_that_fails) {
    std::tuple<std::vector<int>, std::list<double>> t;
    BOOST_REQUIRE_EQUAL(std::size_t(2),
                        stlab::tuple_find(t, [](const auto& t) { return !t.empty(); }));
}

BOOST_AUTO_TEST_CASE(two_element_tuple_first_succeeds) {
    std::tuple<std::vector<int>, std::list<double>> t;
    BOOST_REQUIRE_EQUAL(std::size_t(0),
                        stlab::tuple_find(t, [](const auto& t) { return t.empty(); }));
}

BOOST_AUTO_TEST_CASE(two_element_tuple_second_succeeds) {
    std::tuple<std::vector<int>, std::list<double>> t;
    std::get<0>(t).push_back(0);
    BOOST_REQUIRE_EQUAL(std::size_t(1),
                        stlab::tuple_find(t, [](const auto& t) { return t.empty(); }));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(tuple_for_each_test)

BOOST_AUTO_TEST_CASE(empty_tuple) {
    std::tuple<> t;
    std::size_t count = 0;
    stlab::tuple_for_each(t, [&count](auto&) { ++count; });
    BOOST_REQUIRE_EQUAL(std::size_t(0), count);
}

BOOST_AUTO_TEST_CASE(one_element_tuple) {
    std::tuple<std::vector<int>> t;
    std::get<0>(t) = {
        1, 2}; // workaround for gcc 5 missing capability to support initializer lists for tuples
    std::size_t count = 0;
    stlab::tuple_for_each(t, [&count](auto& c) {
        ++count;
        c.pop_back();
    });
    BOOST_REQUIRE_EQUAL(std::size_t(1), std::get<0>(t).size());
    BOOST_REQUIRE_EQUAL(std::size_t(1), count);
}

BOOST_AUTO_TEST_CASE(two_element_tuple) {
    std::tuple<std::vector<int>, std::list<double>> t;
    std::get<0>(t) = {
        1, 2}; // workaround for gcc 5 missing capability to support initializer lists for tuples
    std::get<1>(t) = {2.4, 3.5};

    std::size_t count = 0;
    stlab::tuple_for_each(t, [&count](auto& c) {
        ++count;
        c.pop_back();
    });
    BOOST_REQUIRE_EQUAL(std::size_t(1), std::get<0>(t).size());
    BOOST_REQUIRE_EQUAL(std::size_t(1), std::get<1>(t).size());
    BOOST_REQUIRE_EQUAL(std::size_t(2), count);
}

BOOST_AUTO_TEST_SUITE_END()
