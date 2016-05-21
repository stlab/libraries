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

using lock_t = std::unique_lock<std::mutex>;

BOOST_FIXTURE_TEST_SUITE(future_when_any_range_void, test_fixture<void>)
    BOOST_AUTO_TEST_CASE(future_when_any_void_void_empty_range) {
        BOOST_TEST_MESSAGE("running future when_all void void with empty range");
        auto check{ false };
        std::vector<stlab::future<void>> emptyFutures;
        sut = when_any(custom_scheduler<0>(), [&_check = check](size_t index) { _check = true;
        }, std::make_pair(emptyFutures.begin(), emptyFutures.end()));

        wait_until_future_fails<std::future_error>(sut);

        BOOST_REQUIRE(!check);
        BOOST_REQUIRE_EQUAL(0, custom_scheduler<0>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_when_any_void_int_empty_range) {
        BOOST_TEST_MESSAGE("running future when_all void int with empty range");
        auto check{ false };
        std::vector<stlab::future<int>> emptyFutures;
        sut = when_any(custom_scheduler<0>(), [&_check = check](int x, size_t index) { _check = true;
        }, std::make_pair(emptyFutures.begin(), emptyFutures.end()));

        wait_until_future_fails<std::future_error>(sut);

        BOOST_REQUIRE(!check);
        BOOST_REQUIRE_EQUAL(0, custom_scheduler<0>::usage_counter());
    }

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

        thread_block_context block_context;
        std::atomic_int task_counter{ 0 };

        std::atomic_int any_task_execution_counter{ 0 };
        size_t used_future_index = 0;
        size_t result = 0;
                
        std::vector<stlab::future<int>> futures;
        futures.push_back(async(custom_scheduler<0>(), [&_context = block_context] { _context._may_proceed = true; return 1; }));
        futures.push_back(async(custom_scheduler<1>(), make_blocking_functor<int>(block_context, task_counter).returns(2)));
        futures.push_back(async(custom_scheduler<0>(), make_blocking_functor<int>(block_context, task_counter).returns(3)));
        futures.push_back(async(custom_scheduler<1>(), make_blocking_functor<int>(block_context, task_counter).returns(5)));
        {
            lock_t block(*block_context._mutex);
            
            sut = when_any(custom_scheduler<0>(), 
                [&_r = result, &_used_future_index = used_future_index, &_counter = any_task_execution_counter](int x, size_t index) {
                _used_future_index = index;
                ++_counter;
                _r = x;
            }, std::make_pair(futures.begin(), futures.end()));
            check_valid_future(sut);
            block_context._go = true;
            wait_until_future_completed(sut);
        }
        block_context._thread_block.notify_all();
        while (task_counter.load() < 2) { std::this_thread::sleep_for(std::chrono::milliseconds(1)); }
        
        BOOST_WARN_EQUAL(size_t(0), used_future_index);
        BOOST_WARN_EQUAL(size_t(1), result);
        BOOST_WARN_EQUAL(1, any_task_execution_counter.load());
        BOOST_WARN_LE(2, custom_scheduler<0>::usage_counter());
        BOOST_WARN_LE(2, custom_scheduler<1>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_when_any_void_range_with_many_elements_middle_succeeds) {
        BOOST_TEST_MESSAGE("running future when_any void with range with many elements and one in the middle succeeds");

        thread_block_context block_context;
        std::atomic_int task_counter{ 0 };

        std::atomic_int any_task_execution_counter{ 0 };
        size_t used_future_index = 0;
        size_t result = 0;

        std::vector<stlab::future<int>> futures;
        futures.push_back(async(custom_scheduler<0>(), make_blocking_functor<int>(block_context, task_counter).returns(1)));
        futures.push_back(async(custom_scheduler<1>(), make_blocking_functor<int>(block_context, task_counter).returns(2)));
        futures.push_back(async(custom_scheduler<0>(), [&_context = block_context] { _context._may_proceed = true; return 3; }));
        futures.push_back(async(custom_scheduler<1>(), make_blocking_functor<int>(block_context, task_counter).returns(5)));

        {
            lock_t lock(*block_context._mutex);
            sut = when_any(custom_scheduler<0>(),
                [&_r = result, &_used_future_index = used_future_index, &_counter = any_task_execution_counter](int x, size_t index) {
                _used_future_index = index;
                ++_counter;
                _r = x;
            }, std::make_pair(futures.begin(), futures.end()));
            check_valid_future(sut);
            block_context._go = true;
            wait_until_future_completed(sut);
        }
        
        block_context._thread_block.notify_all();
        while (task_counter.load() < 2) { std::this_thread::sleep_for(std::chrono::milliseconds(1)); }

        BOOST_REQUIRE_EQUAL(size_t(2), used_future_index);
        BOOST_REQUIRE_EQUAL(size_t(3), result);
        BOOST_REQUIRE_EQUAL(1, any_task_execution_counter.load());
        BOOST_REQUIRE_LE(2, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_LE(2, custom_scheduler<1>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_when_any_void_range_with_many_elements_last_succeeds) {
        BOOST_TEST_MESSAGE("running future when_any void with range with many elements and the last succeeds");

        thread_block_context block_context;
        std::atomic_int task_counter{ 0 };

        std::atomic_int any_task_execution_counter{ 0 };
        size_t used_future_index = 0;
        size_t result = 0;

        std::vector<stlab::future<int>> futures;
        futures.push_back(async(custom_scheduler<0>(), make_blocking_functor<int>(block_context, task_counter).returns(1)));
        futures.push_back(async(custom_scheduler<1>(), make_blocking_functor<int>(block_context, task_counter).returns(2)));
        futures.push_back(async(custom_scheduler<0>(), make_blocking_functor<int>(block_context, task_counter).returns(3)));
        futures.push_back(async(custom_scheduler<1>(), [&_context = block_context] { _context._may_proceed = true; return 5; }));
        {
            lock_t lock(*block_context._mutex);
            sut = when_any(custom_scheduler<0>(),
                [&_r = result, &_used_future_index = used_future_index, &_counter = any_task_execution_counter](int x, size_t index) {
                _used_future_index = index;
                ++_counter;
                _r = x;
            }, std::make_pair(futures.begin(), futures.end()));
            check_valid_future(sut);
            block_context._go = true;
            wait_until_future_completed(sut);
        }

        block_context._thread_block.notify_all();
        while (task_counter.load() < 2) { std::this_thread::sleep_for(std::chrono::milliseconds(1)); }

        BOOST_REQUIRE_EQUAL(size_t(3), used_future_index);
        BOOST_REQUIRE_EQUAL(size_t(5), result);
        BOOST_REQUIRE_EQUAL(1, any_task_execution_counter.load());
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

    BOOST_AUTO_TEST_CASE(future_when_any_void_range_with_many_elements_all_fails) {
        BOOST_TEST_MESSAGE("running future when_any void with range all fails");
        std::atomic_size_t failures{ 0 };
        size_t index = 4711;
        int r = 0;

        std::vector<stlab::future<int>> futures;
        futures.push_back(async(custom_scheduler<1>(), [&_f = failures]()->int { _f += 1; throw test_exception("failure"); }));
        futures.push_back(async(custom_scheduler<0>(), [&_f = failures]()->int { _f += 1; throw test_exception("failure"); }));
        futures.push_back(async(custom_scheduler<1>(), [&_f = failures]()->int { _f += 1; throw test_exception("failure"); }));
        futures.push_back(async(custom_scheduler<0>(), [&_f = failures]()->int { _f += 1; throw test_exception("failure"); }));

        sut = when_any(custom_scheduler<0>(), [&_i = index, &_r = r](int x, size_t index) {
            _i = index;
            _r = x;
        }, std::make_pair(futures.begin(), futures.end()));

        wait_until_future_fails<test_exception>(sut);

        BOOST_REQUIRE_EQUAL(size_t(4711), index);
        BOOST_REQUIRE_EQUAL(0, r);
        BOOST_REQUIRE_GE(size_t(4), failures.load());
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
        {
            std::unique_lock<std::mutex> block(*threadBlock);
            auto start = async(custom_scheduler<0>(), [] { return 4711; });
            std::vector<stlab::future<void>> futures;
            futures.push_back(start.then(custom_scheduler<0>(), [_tb = threadBlock, &_r = r](auto x) { std::unique_lock<std::mutex> lock(*_tb); _r = x + 42; }) );
            futures.push_back( start.then(custom_scheduler<1>(), [_tb = threadBlock, &_r = r](auto x) { std::unique_lock<std::mutex> lock(*_tb); _r = x + 42; }) );
            futures.push_back( start.then(custom_scheduler<0>(), [_tb = threadBlock, &_r = r](auto x) { std::unique_lock<std::mutex> lock(*_tb); _r = x + 42; }) );
            futures.push_back( start.then(custom_scheduler<1>(), [_tb = threadBlock, &_r = r](auto x) { _r = x; }) );

            sut = when_any(custom_scheduler<0>(), [&_r = r, &_i = index](size_t index) {
                _r += _r;
                _i = index;
            }, std::make_pair(futures.begin(), futures.end()));
            check_valid_future(sut);

            wait_until_future_completed(sut);

            BOOST_REQUIRE_EQUAL(4711 + 4711, r.load());
        }
        BOOST_REQUIRE_EQUAL(size_t(3), index);
        BOOST_REQUIRE_LE(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
    }
BOOST_AUTO_TEST_SUITE_END()


BOOST_FIXTURE_TEST_SUITE(future_when_any_range_int, test_fixture<int>)
    BOOST_AUTO_TEST_CASE(future_when_any_int_empty_range) {
        BOOST_TEST_MESSAGE("running future when_any int with empty range");

        auto check{ false };
        std::vector<stlab::future<int>> emptyFutures;
        sut = when_any(custom_scheduler<0>(), [&_check = check](int x, size_t index) {
            _check = true;
            return x + 42;
        }, std::make_pair(emptyFutures.begin(), emptyFutures.end()));

        wait_until_future_fails<std::future_error>(sut);

        BOOST_REQUIRE(!check);
    }

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
        
        thread_block_context block_context;
        std::atomic_int task_counter{ 0 };

        std::atomic_int any_task_execution_counter{ 0 };
        size_t used_future_index = 0;

        std::vector<stlab::future<int>> futures;
        futures.push_back(async(custom_scheduler<0>(), make_blocking_functor<int>(block_context, task_counter).returns(42)));
        futures.push_back(async(custom_scheduler<1>(), make_blocking_functor<int>(block_context, task_counter).returns(815)));
        futures.push_back(async(custom_scheduler<0>(), make_blocking_functor<int>(block_context, task_counter).returns(4711)));
        futures.push_back(async(custom_scheduler<1>(), [&_context = block_context] { _context._may_proceed = true; return 5; }));

        {
            lock_t lock(*block_context._mutex);
            sut = when_any(custom_scheduler<0>(),
                [&_used_future_index = used_future_index, &_counter = any_task_execution_counter](int x, size_t index) {
                _used_future_index = index;
                ++_counter;
                return x;
            }, std::make_pair(futures.begin(), futures.end()));
            check_valid_future(sut);
            block_context._go = true;
            wait_until_future_completed(sut);
        }
        block_context._thread_block.notify_all();
        while (task_counter.load() < 2) { std::this_thread::sleep_for(std::chrono::milliseconds(1)); }

        BOOST_REQUIRE_EQUAL(size_t(3), used_future_index);
        BOOST_REQUIRE_EQUAL(5, *sut.get_try());
        BOOST_REQUIRE_EQUAL(1, any_task_execution_counter.load());
        BOOST_REQUIRE_LE(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_LE(3, custom_scheduler<1>::usage_counter());
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
        {
            std::unique_lock<std::mutex> block(*threadBlock);
            auto start = async(custom_scheduler<0>(), [] { return 4711; });
            std::vector<stlab::future<int>> futures;
            futures.push_back( start.then(custom_scheduler<0>(), [_tb = threadBlock](auto x) { std::unique_lock<std::mutex> lock(*_tb); return x + 1; }) );
            futures.push_back( start.then(custom_scheduler<1>(), [](auto x) { return x + 2; }) );
            futures.push_back( start.then(custom_scheduler<0>(), [_tb = threadBlock](auto x) { std::unique_lock<std::mutex> lock(*_tb); return x + 3; }) );
            futures.push_back( start.then(custom_scheduler<1>(), [_tb = threadBlock](auto x) { std::unique_lock<std::mutex> lock(*_tb); return x + 5; }) );

            sut = when_any(custom_scheduler<0>(), [&_i = index](int x, size_t index) {
                _i = index;
                return x;
            }, std::make_pair(futures.begin(), futures.end()));
            check_valid_future(sut);
            wait_until_future_completed(sut);
        }

        BOOST_REQUIRE_EQUAL(size_t(1), index);
        BOOST_REQUIRE_EQUAL(4711 + 2, *sut.get_try());
        BOOST_REQUIRE_LE(2, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_LE(2, custom_scheduler<1>::usage_counter());
    }

BOOST_AUTO_TEST_SUITE_END()
