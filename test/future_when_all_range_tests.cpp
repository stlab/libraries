/*
Copyright 2016 Felix Petriconi
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <stlab/future.hpp>
#include <array>
#include "test_helper.hpp"

using namespace stlab;
using namespace test_helper;

BOOST_FIXTURE_TEST_SUITE(future_when_all_range_void, success_fixture<void>)
    BOOST_AUTO_TEST_CASE(future_when_all_void_empty_range) {
        BOOST_TEST_MESSAGE("running future when_all void with empty range");
        size_t p = 0;
        std::vector<stlab::future<int>> emptyFutures;
        sut = when_all(custom_scheduler<0>(), [_p = &p](std::vector<int> v) { *_p = v.size();
            }, std::make_pair(emptyFutures.begin(), emptyFutures.end()));

        check_valid_future(sut);
        wait_until_future_completed(sut);

        BOOST_REQUIRE_EQUAL(0, p);
        BOOST_REQUIRE_LE(1, custom_scheduler<0>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_when_all_void_range_with_one_element) {
        BOOST_TEST_MESSAGE("running future when_all void with range of one element");
        size_t p = 0;
        size_t r = 0;
        std::vector<stlab::future<int>> futures;
        futures.push_back(async(custom_scheduler<0>(), [] { return 42; }));

        sut = when_all(custom_scheduler<0>(), [_p = &p, _r = &r](std::vector<int> v) { 
            *_p = v.size();
            *_r = v[0];
        }, std::make_pair(futures.begin(), futures.end()));

        check_valid_future(sut);
        wait_until_future_completed(sut);

        BOOST_REQUIRE_EQUAL(1, p);
        BOOST_REQUIRE_EQUAL(42, r);
        BOOST_REQUIRE_LE(2, custom_scheduler<0>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_when_all_void_range_with_many_elements) {
        BOOST_TEST_MESSAGE("running future when_all void with range with many elements");
        size_t p = 0;
        size_t r = 0;
        std::vector<stlab::future<int>> futures;
        futures.push_back(async(custom_scheduler<0>(), [] { return 1; }));
        futures.push_back(async(custom_scheduler<1>(), [] { return 2; }));
        futures.push_back(async(custom_scheduler<0>(), [] { return 3; }));
        futures.push_back(async(custom_scheduler<1>(), [] { return 5; }));

        sut = when_all(custom_scheduler<0>(), [_p = &p, _r = &r](std::vector<int> v) {
                *_p = v.size();
                for (auto i : v) {
                    *_r += i;
                }
            }, std::make_pair(futures.begin(), futures.end()));
        check_valid_future(sut);

        wait_until_future_completed(sut);

        BOOST_REQUIRE_EQUAL(4, p);
        BOOST_REQUIRE_EQUAL(1 + 2 + 3 + 5, r);
        BOOST_REQUIRE_LE(2, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_LE(2, custom_scheduler<1>::usage_counter());
    }

    
    /*
           /  F1  \
          / / F2 \ \
    start           sut
          \ \ F3 / /
           \  F4  /
    */
    BOOST_AUTO_TEST_CASE(future_when_all_void_range_with_diamond_formation_elements) {
        BOOST_TEST_MESSAGE("running future when_all void with range with diamond formation");
        int v[4] = { 0, 0, 0, 0 };
        int r = 0;

        auto start = async(custom_scheduler<0>(), [] { return 4711; });
        std::vector<stlab::future<void>> futures(4);
        futures[0] = start.then(custom_scheduler<0>(), [_p = &v[0]](auto x) { *_p = x + 1; });
        futures[1] = start.then(custom_scheduler<1>(), [_p = &v[1]](auto x) { *_p = x + 2; });
        futures[2] = start.then(custom_scheduler<0>(), [_p = &v[2]](auto x) { *_p = x + 3; });
        futures[3] = start.then(custom_scheduler<1>(), [_p = &v[3]](auto x) { *_p = x + 5; });

        sut = when_all(custom_scheduler<0>(), [_r = &r, &v]() {
            for (auto i : v) {
                *_r += i;
            }
        }, std::make_pair(futures.begin(), futures.end()));
        check_valid_future(sut);

        wait_until_future_completed(sut);

        BOOST_REQUIRE_EQUAL(4711+1 + 4711+2 +  4711+3 + 4711+5, r);
        BOOST_REQUIRE_LE(2, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_LE(2, custom_scheduler<1>::usage_counter());
    }
    
    
BOOST_AUTO_TEST_SUITE_END()


BOOST_FIXTURE_TEST_SUITE(future_when_all_range_int, success_fixture<int>)
    BOOST_AUTO_TEST_CASE(future_when_all_int_empty_range) {
        BOOST_TEST_MESSAGE("running future when_all int with empty range");
        
        std::vector<stlab::future<int>> emptyFutures;
        sut = when_all(custom_scheduler<0>(), [](std::vector<int> v) { 
                return static_cast<int>(v.size());
            }, std::make_pair(emptyFutures.begin(), emptyFutures.end()));
        check_valid_future(sut);

        wait_until_future_completed(sut);

        BOOST_REQUIRE_EQUAL(0, *sut.get_try());
        BOOST_REQUIRE_LE(1, custom_scheduler<0>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_when_all_int_range_with_one_element) {
        BOOST_TEST_MESSAGE("running future when_all int with range of one element");
        size_t p = 0;

        std::vector<stlab::future<int>> futures;
        futures.push_back(async(custom_scheduler<0>(), [] { return 42; }));

        sut = when_all(custom_scheduler<0>(), [_p = &p](std::vector<int> v) {
                *_p = v.size();
                return v[0];
            }, std::make_pair(futures.begin(), futures.end()));
        check_valid_future(sut);

        wait_until_future_completed(sut);

        BOOST_REQUIRE_EQUAL(1, p);
        BOOST_REQUIRE_EQUAL(42, *sut.get_try());
        BOOST_REQUIRE_LE(1, custom_scheduler<0>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_when_all_int_range_with_many_elements) {
        BOOST_TEST_MESSAGE("running future when_all int with range with many elements");
        size_t p = 0;
        std::vector<stlab::future<int>> futures;
        futures.push_back(async(custom_scheduler<0>(), [] { return 1; }));
        futures.push_back(async(custom_scheduler<1>(), [] { return 2; }));
        futures.push_back(async(custom_scheduler<0>(), [] { return 3; }));
        futures.push_back(async(custom_scheduler<1>(), [] { return 5; }));

        sut = when_all(custom_scheduler<0>(), [_p = &p](std::vector<int> v) {
            *_p = v.size();
            auto r = 0;
            for (auto i : v) {
                r += i;
            }
            return r;
        }, std::make_pair(futures.begin(), futures.end()));
        check_valid_future(sut);

        wait_until_future_completed(sut);

        BOOST_REQUIRE_EQUAL(4, p);
        BOOST_REQUIRE_EQUAL(1 + 2 + 3 + 5, *sut.get_try());
        BOOST_REQUIRE_LE(2, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_LE(2, custom_scheduler<1>::usage_counter());
    }

    /*
           /  F1  \
          / / F2 \ \
    start           sut
          \ \ F3 / /
           \  F4  /
    */
    BOOST_AUTO_TEST_CASE(future_when_all_int_range_with_diamond_formation_elements) {
        BOOST_TEST_MESSAGE("running future when_all int with range with diamond formation");
        size_t p = 0;

        auto start = async(custom_scheduler<0>(), [] { return 4711; });
        std::vector<stlab::future<int>> futures(4);
        futures[0] = start.then(custom_scheduler<0>(), [](auto x) { return x + 1; });
        futures[1] = start.then(custom_scheduler<1>(), [](auto x) { return x + 2; });
        futures[2] = start.then(custom_scheduler<0>(), [](auto x) { return x + 3; });
        futures[3] = start.then(custom_scheduler<1>(), [](auto x) { return x + 5; });

        sut = when_all(custom_scheduler<0>(), [_p = &p](std::vector<int> v) {
            *_p = v.size();
            auto r = 0;
            for (auto i : v) {
                r += i;
            }
            return r;
        }, std::make_pair(futures.begin(), futures.end()));
        check_valid_future(sut);

        wait_until_future_completed(sut);

        BOOST_REQUIRE_EQUAL(4, p);
        BOOST_REQUIRE_EQUAL(4711+1 + 4711+2 +  4711+3 + 4711+5, *sut.get_try());
        BOOST_REQUIRE_LE(2, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_LE(2, custom_scheduler<1>::usage_counter());
    }


BOOST_AUTO_TEST_SUITE_END()