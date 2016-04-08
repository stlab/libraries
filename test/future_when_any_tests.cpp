/*
Copyright 2016 Felix Petriconi
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <stlab/future.hpp>

#include "test_helper.hpp"

using namespace stlab;
using namespace test_helper;

BOOST_FIXTURE_TEST_SUITE(future_when_any_range_void, success_fixture<void>)
#if 0 
    // REVISIT what to do in this case
    BOOST_AUTO_TEST_CASE(future_when_any_void_empty_range) {
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
#endif
    BOOST_AUTO_TEST_CASE(future_when_any_void_range_with_one_element) {
        BOOST_TEST_MESSAGE("running future when_any void with range of one element");
        size_t index = 4711;
        size_t r = 0;
        std::vector<stlab::future<int>> futures;
        futures.push_back(async(custom_scheduler<0>(), [] { return 42; }));

        sut = when_any(custom_scheduler<0>(), [&_i = index, &_r = r](int x, size_t index) {
            _i = index;
            _r = x;
        }, std::make_pair(futures.begin(), futures.end()));

        check_valid_future(sut);
        wait_until_future_completed(sut);

        BOOST_REQUIRE_EQUAL(size_t(0), index);
        BOOST_REQUIRE_EQUAL(size_t(42), r);
        BOOST_REQUIRE_LE(2, custom_scheduler<0>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_when_any_void_range_with_many_elements_first_succeeds) {
        BOOST_TEST_MESSAGE("running future when_any void with range with many elements and the first succeeds");
        size_t index = 4711;
        size_t r = 0;
        auto threadBlock = std::make_shared<std::mutex>();
        threadBlock->lock();
        std::vector<stlab::future<int>> futures;
        futures.push_back(async(custom_scheduler<0>(), [] { return 1; }));
        futures.push_back(async(custom_scheduler<1>(), [&_tb = threadBlock] { std::unique_lock<std::mutex> block(*_tb); return 2; }));
        futures.push_back(async(custom_scheduler<0>(), [&_tb = threadBlock] { std::unique_lock<std::mutex> block(*_tb); return 3; }));
        futures.push_back(async(custom_scheduler<1>(), [&_tb = threadBlock] { std::unique_lock<std::mutex> block(*_tb); return 5; }));

        sut = when_any(custom_scheduler<0>(), [&_i = index, &_r = r](int x, size_t index) {
            _i = index;
            _r = x;
        }, std::make_pair(futures.begin(), futures.end()));
        check_valid_future(sut);

        wait_until_future_completed(sut);
        threadBlock->unlock();

        BOOST_REQUIRE_EQUAL(size_t(0), index);
        BOOST_REQUIRE_EQUAL(size_t(1), r);
        BOOST_REQUIRE_LE(2, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_LE(2, custom_scheduler<1>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_when_any_void_range_with_many_elements_middle_succeeds) {
        BOOST_TEST_MESSAGE("running future when_any void with range with many elements and one in the middle succeeds");
        size_t index = 4711;
        size_t r = 0;
        auto threadBlock = std::make_shared<std::mutex>();
        threadBlock->lock();
        std::vector<stlab::future<int>> futures;
        futures.push_back(async(custom_scheduler<1>(), [&_tb = threadBlock] { std::unique_lock<std::mutex> block(*_tb); return 1; }));
        futures.push_back(async(custom_scheduler<0>(), [&_tb = threadBlock] { std::unique_lock<std::mutex> block(*_tb); return 2; }));
        futures.push_back(async(custom_scheduler<1>(), [] { return 3; }));
        futures.push_back(async(custom_scheduler<0>(), [_tb = threadBlock] { std::unique_lock<std::mutex> block(*_tb); return 5; }));

        sut = when_any(custom_scheduler<0>(), [&_i = index, &_r = r](int x, size_t index) {
            _i = index;
            _r = x;
        }, std::make_pair(futures.begin(), futures.end()));
        check_valid_future(sut);

        wait_until_future_completed(sut);
        threadBlock->unlock();

        BOOST_REQUIRE_EQUAL(size_t(2), index);
        BOOST_REQUIRE_EQUAL(size_t(3), r);
        BOOST_REQUIRE_LE(2, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_LE(2, custom_scheduler<1>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_when_any_void_range_with_many_elements_one_succeeds_all_other_fails) {
        BOOST_TEST_MESSAGE("running future when_any void with range with many elements and one succeeds all other fails");
        std::atomic_size_t failures{ 0 };
        size_t index = 4711;
        int r = 0;

        std::vector<stlab::future<int>> futures;
        futures.push_back(async(custom_scheduler<1>(), [&_f = failures]()->int { _f += 1; throw test_exception("failure"); }));
        futures.push_back(async(custom_scheduler<0>(), [&_f = failures]()->int { _f += 1; throw test_exception("failure"); }));
        futures.push_back(async(custom_scheduler<1>(), [] { return 3; }));
        futures.push_back(async(custom_scheduler<0>(), [&_f = failures]()->int { _f += 1; throw test_exception("failure"); }));

        sut = when_any(custom_scheduler<0>(), [&_i = index, &_r = r](int x, size_t index) {
            _i = index;
            _r = x;
        }, std::make_pair(futures.begin(), futures.end()));
        check_valid_future(sut);

        wait_until_future_completed(sut);

        BOOST_REQUIRE_EQUAL(size_t(2), index);
        BOOST_REQUIRE_EQUAL(3, r);
        BOOST_REQUIRE_GE(size_t(3), failures.load());
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
    BOOST_AUTO_TEST_CASE(future_when_any_void_range_with_diamond_formation_elements) {
        BOOST_TEST_MESSAGE("running future when_any void with range with diamond formation");
        auto threadBlock = std::make_shared<std::mutex>();
        std::atomic_int r{ 0 };
        size_t index = 0;
        threadBlock->lock();
        auto start = async(custom_scheduler<0>(), [] { return 4711; });
        std::vector<stlab::future<void>> futures(4);
        futures[0] = start.then(custom_scheduler<0>(), [&_tb = threadBlock, &_r = r](auto x) { std::unique_lock<std::mutex> lock(*_tb); _r = x + 42; }); 
        futures[1] = start.then(custom_scheduler<1>(), [&_tb = threadBlock, &_r = r](auto x) { std::unique_lock<std::mutex> lock(*_tb); _r = x + 42; });
        futures[2] = start.then(custom_scheduler<0>(), [&_tb = threadBlock, &_r = r](auto x) { std::unique_lock<std::mutex> lock(*_tb); _r = x + 42; });
        futures[3] = start.then(custom_scheduler<1>(), [&_tb = threadBlock, &_r = r](auto x) { _r = x; }); 

        sut = when_any(custom_scheduler<0>(), [&_r = r, &_i = index](size_t index) {
            _r += _r;
            _i = index;
        }, std::make_pair(futures.begin(), futures.end()));
        check_valid_future(sut);

        wait_until_future_completed(sut);

        BOOST_REQUIRE_EQUAL(4711 + 4711, r.load());
        threadBlock->unlock();
        BOOST_REQUIRE_EQUAL(size_t(3), index);
        BOOST_REQUIRE_LE(2, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_LE(2, custom_scheduler<1>::usage_counter());
    }
BOOST_AUTO_TEST_SUITE_END()


BOOST_FIXTURE_TEST_SUITE(future_when_any_range_int, success_fixture<int>)
#if 0
BOOST_AUTO_TEST_CASE(future_when_any_int_empty_range) {
        BOOST_TEST_MESSAGE("running future when_any int with empty range");

        std::vector<stlab::future<int>> emptyFutures; // REVISIT Fp what to do with empty range?
        sut = when_any(custom_scheduler<0>(), [](int x, size_t index) {
            return static_cast<int>(v.size());
        }, std::make_pair(emptyFutures.begin(), emptyFutures.end()));
        check_valid_future(sut);

        wait_until_future_completed(sut);

        BOOST_REQUIRE_EQUAL(0, *sut.get_try());
        BOOST_REQUIRE_LE(1, custom_scheduler<0>::usage_counter());
    }
#endif

    BOOST_AUTO_TEST_CASE(future_when_any_int_range_with_one_element) {
        BOOST_TEST_MESSAGE("running future when_any int with range of one element");
        size_t index = 42;

        std::vector<stlab::future<int>> futures;
        futures.push_back(async(custom_scheduler<0>(), [] { return 4711; }));

        sut = when_any(custom_scheduler<0>(), [&_i = index](int x, size_t index) {
            _i = index;
            return x;
        }, std::make_pair(futures.begin(), futures.end()));
        check_valid_future(sut);

        wait_until_future_completed(sut);

        BOOST_REQUIRE_EQUAL(size_t(0), index);
        BOOST_REQUIRE_EQUAL(4711, *sut.get_try());
        BOOST_REQUIRE_LE(1, custom_scheduler<0>::usage_counter());
    }


    BOOST_AUTO_TEST_CASE(future_when_any_int_range_with_many_elements) {
        BOOST_TEST_MESSAGE("running future when_any int with range with many elements and the last suceeds");
        size_t index = 0;
        auto threadBlock = std::make_shared<std::mutex>();
        threadBlock->lock();
        std::vector<stlab::future<int>> futures;
        futures.push_back(async(custom_scheduler<0>(), [&_tb = threadBlock] { std::unique_lock<std::mutex> lock(*_tb); return 1; }));
        futures.push_back(async(custom_scheduler<1>(), [&_tb = threadBlock] { std::unique_lock<std::mutex> lock(*_tb); return 2; }));
        futures.push_back(async(custom_scheduler<0>(), [&_tb = threadBlock] { std::unique_lock<std::mutex> lock(*_tb); return 3; }));
        futures.push_back(async(custom_scheduler<1>(), [] { return 5; }));

        sut = when_any(custom_scheduler<0>(), [&_index = index](int x, size_t index) {
            _index = index;
            return x;
        }, std::make_pair(futures.begin(), futures.end()));
        check_valid_future(sut);

        wait_until_future_completed(sut);
        threadBlock->unlock();

        BOOST_REQUIRE_EQUAL(size_t(3), index);
        BOOST_REQUIRE_EQUAL(5, *sut.get_try());
        BOOST_REQUIRE_LE(2, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_LE(2, custom_scheduler<1>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_when_any_int_range_with_many_elements_all_but_all_one_failing) {
        BOOST_TEST_MESSAGE("running future when_any int with range with many elements all but one is failing");
        size_t index = 0;
        std::atomic_int failures{ 0 };
        std::vector<stlab::future<int>> futures;
        futures.push_back(async(custom_scheduler<0>(), [&_f = failures]()->int { _f += 1; throw test_exception("failure"); }));
        futures.push_back(async(custom_scheduler<1>(), [&_f = failures]()->int { _f += 1; throw test_exception("failure"); }));
        futures.push_back(async(custom_scheduler<0>(), []()->int { return 3; }));
        futures.push_back(async(custom_scheduler<1>(), [&_f = failures]()->int { _f += 1; throw test_exception("failure"); }));

        sut = when_any(custom_scheduler<0>(), [&_index = index](int x, size_t index) {
            _index = index;
            return x;
        }, std::make_pair(futures.begin(), futures.end()));
        check_valid_future(sut);

        wait_until_future_completed(sut);

        BOOST_REQUIRE_EQUAL(size_t(2), index);
        BOOST_REQUIRE_EQUAL(3, *sut.get_try());
        BOOST_REQUIRE_GE(3, failures.load());
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
    BOOST_AUTO_TEST_CASE(future_when_any_int_range_with_diamond_formation_elements) {
        BOOST_TEST_MESSAGE("running future when_any int with range with diamond formation");
        size_t index = 0;
        auto threadBlock = std::make_shared<std::mutex>();
        threadBlock->lock();

        auto start = async(custom_scheduler<0>(), [] { return 4711; });
        std::vector<stlab::future<int>> futures(4);
        futures[0] = start.then(custom_scheduler<0>(), [&_tb = threadBlock](auto x) { std::unique_lock<std::mutex> lock(*_tb); return x + 1; });
        futures[1] = start.then(custom_scheduler<1>(), [](auto x) { return x + 2; });
        futures[2] = start.then(custom_scheduler<0>(), [&_tb = threadBlock](auto x) { std::unique_lock<std::mutex> lock(*_tb); return x + 3; });
        futures[3] = start.then(custom_scheduler<1>(), [&_tb = threadBlock](auto x) { std::unique_lock<std::mutex> lock(*_tb); return x + 5; });

        sut = when_any(custom_scheduler<0>(), [&_i = index](int x, size_t index) {
            _i = index;
            return x;
        }, std::make_pair(futures.begin(), futures.end()));
        check_valid_future(sut);

        wait_until_future_completed(sut);
        threadBlock->unlock();

        BOOST_REQUIRE_EQUAL(size_t(1), index);
        BOOST_REQUIRE_EQUAL(4711 + 2, *sut.get_try());
        BOOST_REQUIRE_LE(2, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_LE(2, custom_scheduler<1>::usage_counter());
    }

BOOST_AUTO_TEST_SUITE_END()
