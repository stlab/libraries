/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#include <atomic>
#include <cstddef>
#include <mutex>
#include <utility>

#include <boost/test/unit_test.hpp>

#include <stlab/concurrency/await.hpp>
#include <stlab/concurrency/future.hpp>
#include <stlab/test/model.hpp>
#include <stlab/utility.hpp>

#include "future_test_helper.hpp"

using namespace stlab;
using namespace future_test_helper;

using lock_t = std::unique_lock<std::mutex>;

BOOST_FIXTURE_TEST_SUITE(future_when_any_arg_void, test_fixture<void>)
BOOST_AUTO_TEST_CASE(future_when_any_int_void_range_with_one_argument) {
    BOOST_TEST_MESSAGE("running future when_any int void with range of one argument");
    size_t index = 4711;
    size_t result = 0;
    auto a1 = async(make_executor<0>(), [] { return 42; });

    sut = when_any(
        make_executor<1>(),
        [&_i = index, &_r = result](int x, size_t index) {
            _i = index;
            _r = x;
        },
        std::move(a1));

    check_valid_future(sut);
    wait_until_future_completed(std::move(sut));

    BOOST_REQUIRE_EQUAL(size_t(0), index);
    BOOST_REQUIRE_EQUAL(size_t(42), result);
    BOOST_REQUIRE_LE(1, custom_scheduler<0>::usage_counter());
    BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_CASE(future_when_any_int_void_range_with_many_arguments_first_succeeds) {
    BOOST_TEST_MESSAGE(
        "running future when_any int void with range with many arguments and the first succeeds");

    thread_block_context block_context;
    std::atomic_int any_task_execution_counter{0};
    size_t used_future_index = 0;
    size_t result = 0;

    auto a1 = async(make_executor<0>(), make_non_blocking_functor(
                                            [&_context = block_context]() {
                                                _context._may_proceed = true;
                                                return 1;
                                            },
                                            _task_counter));
    auto a2 = async(make_executor<0>(),
                    make_blocking_functor([] { return 2; }, _task_counter, block_context));
    auto a3 = async(make_executor<0>(),
                    make_blocking_functor([] { return 3; }, _task_counter, block_context));
    auto a4 = async(make_executor<0>(),
                    make_blocking_functor([] { return 5; }, _task_counter, block_context));
    {
        lock_t const block(*block_context._mutex);

        sut = when_any(
            make_executor<1>(),
            [&_r = result, &_used_future_index = used_future_index,
             &_counter = any_task_execution_counter](int x, size_t index) {
                _used_future_index = index;
                ++_counter;
                _r = x;
            },
            std::move(a1), std::move(a2), std::move(a3), std::move(a4));

        check_valid_future(sut);
        wait_until_future_completed(std::move(sut));
        block_context._go = true;
    }

    block_context._thread_block.notify_all();
    wait_until_all_tasks_completed();

    BOOST_REQUIRE_EQUAL(size_t(0), used_future_index);
    BOOST_REQUIRE_EQUAL(size_t(1), result);
    BOOST_REQUIRE_EQUAL(1, any_task_execution_counter.load());
    BOOST_REQUIRE_LE(1, custom_scheduler<0>::usage_counter());
    BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_CASE(future_when_any_int_void_argument_with_many_arguments_middle_succeeds) {
    BOOST_TEST_MESSAGE(
        "running future when_any int void argument with many arguments and one in the middle succeeds");

    thread_block_context block_context;
    std::atomic_int any_task_execution_counter{0};
    size_t used_future_index = 0;
    size_t result = 0;

    auto a1 = async(make_executor<0>(),
                    make_blocking_functor([] { return 1; }, _task_counter, block_context));
    auto a2 = async(make_executor<0>(),
                    make_blocking_functor([] { return 2; }, _task_counter, block_context));
    auto a3 = async(make_executor<0>(), make_non_blocking_functor(
                                            [&_context = block_context] {
                                                _context._may_proceed = true;
                                                return 3;
                                            },
                                            _task_counter));
    auto a4 = async(make_executor<0>(),
                    make_blocking_functor([] { return 5; }, _task_counter, block_context));

    {
        lock_t const lock(*block_context._mutex);

        sut = when_any(
            make_executor<1>(),
            [&_r = result, &_used_future_index = used_future_index,
             &_counter = any_task_execution_counter](int x, size_t index) {
                _used_future_index = index;
                ++_counter;
                _r = x;
            },
            std::move(a1), std::move(a2), std::move(a3), std::move(a4));

        check_valid_future(sut);
        wait_until_future_completed(std::move(sut));
        block_context._go = true;
    }

    block_context._thread_block.notify_all();
    wait_until_all_tasks_completed();

    BOOST_REQUIRE_EQUAL(size_t(2), used_future_index);
    BOOST_REQUIRE_EQUAL(size_t(3), result);
    BOOST_REQUIRE_EQUAL(1, any_task_execution_counter.load());
    BOOST_REQUIRE_LE(4, custom_scheduler<0>::usage_counter());
    BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_CASE(future_when_any_int_void_argument_with_many_arguments_last_succeeds) {
    BOOST_TEST_MESSAGE(
        "running future when_any int void arguments with many arguments and the last succeeds");

    thread_block_context block_context;
    std::atomic_int any_task_execution_counter{0};
    size_t used_future_index = 0;
    size_t result = 0;

    auto a1 = async(make_executor<0>(),
                    make_blocking_functor([] { return 1; }, _task_counter, block_context));
    auto a2 = async(make_executor<0>(),
                    make_blocking_functor([] { return 2; }, _task_counter, block_context));
    auto a3 = async(make_executor<0>(),
                    make_blocking_functor([] { return 3; }, _task_counter, block_context));
    auto a4 = async(make_executor<0>(), make_non_blocking_functor(
                                            [&_context = block_context] {
                                                _context._may_proceed = true;
                                                return 5;
                                            },
                                            _task_counter));
    {
        lock_t const lock(*block_context._mutex);

        sut = when_any(
            make_executor<1>(),
            [&_r = result, &_used_future_index = used_future_index,
             &_counter = any_task_execution_counter](int x, size_t index) {
                _used_future_index = index;
                ++_counter;
                _r = x;
            },
            std::move(a1), std::move(a2), std::move(a3), std::move(a4));

        check_valid_future(sut);
        wait_until_future_completed(std::move(sut));
        block_context._go = true;
    }

    block_context._thread_block.notify_all();
    wait_until_all_tasks_completed();

    BOOST_REQUIRE_EQUAL(size_t(3), used_future_index);
    BOOST_REQUIRE_EQUAL(size_t(5), result);
    BOOST_REQUIRE_EQUAL(1, any_task_execution_counter.load());
    BOOST_REQUIRE_LE(4, custom_scheduler<0>::usage_counter());
    BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_CASE(
    future_when_any_int_void_arguments_with_many_arguments_one_succeeds_all_other_fails) {
    BOOST_TEST_MESSAGE(
        "running future when_any int void arguments with many arguments and one succeeds all other fails");

    std::atomic_int any_task_execution_counter{0};
    size_t index = 4711;
    int result = 0;

    auto a1 = async(make_executor<0>(), make_failing_functor([] { return 1; }, _task_counter));
    auto a2 = async(make_executor<0>(), make_failing_functor([] { return 1; }, _task_counter));
    auto a3 = async(make_executor<0>(), make_non_blocking_functor([] { return 3; }, _task_counter));
    auto a4 = async(make_executor<0>(), make_failing_functor([] { return 1; }, _task_counter));

    sut = when_any(
        make_executor<1>(),
        [&_i = index, &_result = result, &_counter = any_task_execution_counter](int x,
                                                                                 size_t index) {
            ++_counter;
            _i = index;
            _result = x;
        },
        std::move(a1), std::move(a2), std::move(a3), std::move(a4));

    check_valid_future(sut);
    wait_until_future_completed(std::move(sut));
    wait_until_all_tasks_completed();

    BOOST_REQUIRE_EQUAL(size_t(2), index);
    BOOST_REQUIRE_EQUAL(3, result);
    BOOST_REQUIRE_EQUAL(1, any_task_execution_counter.load());
    BOOST_REQUIRE_LE(4, custom_scheduler<0>::usage_counter());
    BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_CASE(future_when_any_int_void_arguments_with_many_arguments_all_fail) {
    BOOST_TEST_MESSAGE("running future when_any int void arguments all fail");
    std::atomic_size_t const failures{0};
    size_t index = 4711;
    int r = 0;

    auto a1 = async(make_executor<0>(), make_failing_functor([] { return 0; }, _task_counter));
    auto a2 = async(make_executor<0>(), make_failing_functor([] { return 0; }, _task_counter));
    auto a3 = async(make_executor<0>(), make_failing_functor([] { return 0; }, _task_counter));
    auto a4 = async(make_executor<0>(), make_failing_functor([] { return 0; }, _task_counter));

    sut = when_any(
        make_executor<1>(),
        [&_i = index, &_r = r](int x, size_t index) {
            _i = index;
            _r = x;
        },
        std::move(a1), std::move(a2), std::move(a3), std::move(a4));

    wait_until_all_tasks_completed();
    wait_until_future_fails<test_exception>(std::move(sut));

    BOOST_REQUIRE_EQUAL(size_t(4711), index);
    BOOST_REQUIRE_EQUAL(0, r);
    BOOST_WARN_GE(size_t(4), failures.load());
    BOOST_REQUIRE_LE(4, custom_scheduler<0>::usage_counter());
    BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(future_when_any_argument_int, test_fixture<int>)
BOOST_AUTO_TEST_CASE(future_when_any_int_int_argument_with_one_argument) {
    BOOST_TEST_MESSAGE("running future when_any int int arguments of one argument");
    size_t index = 42;

    auto a1 = async(make_executor<0>(), [] { return 4711; });

    sut = when_any(
        make_executor<1>(),
        [&_i = index](int x, size_t index) {
            _i = index;
            return x;
        },
        std::move(a1));
    check_valid_future(sut);

    auto result = await(std::move(sut));

    BOOST_REQUIRE_EQUAL(size_t(0), index);
    BOOST_REQUIRE_EQUAL(4711, result);
    BOOST_REQUIRE_LE(1, custom_scheduler<0>::usage_counter());
    BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_CASE(future_when_any_int_int_arguments_with_many_arguments_last_succeeds) {
    BOOST_TEST_MESSAGE(
        "running future when_any int int arguments with many elements and the last suceeds");

    thread_block_context block_context;

    std::atomic_int any_task_execution_counter{0};
    size_t used_future_index = 0;

    auto a1 = async(make_executor<0>(),
                    make_blocking_functor([] { return 42; }, _task_counter, block_context));
    auto a2 = async(make_executor<0>(),
                    make_blocking_functor([] { return 815; }, _task_counter, block_context));
    auto a3 = async(make_executor<0>(),
                    make_blocking_functor([] { return 4711; }, _task_counter, block_context));
    auto a4 = async(make_executor<0>(), make_non_blocking_functor(
                                            [&_context = block_context] {
                                                _context._may_proceed = true;
                                                return 5;
                                            },
                                            _task_counter));

    {
        lock_t const lock(*block_context._mutex);
        sut = when_any(
            make_executor<1>(),
            [&_used_future_index = used_future_index,
             &_counter = any_task_execution_counter](int x, size_t index) {
                _used_future_index = index;
                ++_counter;
                return x;
            },
            std::move(a1), std::move(a2), std::move(a3), std::move(a4));
        check_valid_future(sut);
        wait_until_future_completed(copy(sut));
        block_context._go = true;
    }
    block_context._thread_block.notify_all();
    wait_until_all_tasks_completed();

    BOOST_REQUIRE_EQUAL(size_t(3), used_future_index);
    BOOST_REQUIRE_EQUAL(5, *sut.get_try());
    BOOST_REQUIRE_EQUAL(1, any_task_execution_counter.load());
    BOOST_REQUIRE_LE(4, custom_scheduler<0>::usage_counter());
    BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_CASE(
    future_when_any_int_int_arguments_with_many_arguments_all_but_all_one_failing) {
    BOOST_TEST_MESSAGE(
        "running future when_any int int arguments with many arguments all but one is failing");
    size_t index = 0;
    std::atomic_int failures{0};

    auto a1 = async(make_executor<0>(), make_failing_functor(
                                            [&_f = failures]() -> int {
                                                ++_f;
                                                return 0;
                                            },
                                            _task_counter));
    auto a2 = async(make_executor<0>(), make_failing_functor(
                                            [&_f = failures]() -> int {
                                                ++_f;
                                                return 0;
                                            },
                                            _task_counter));
    auto a3 = async(make_executor<0>(),
                    make_non_blocking_functor([]() -> int { return 3; }, _task_counter));
    auto a4 = async(make_executor<0>(), make_failing_functor(
                                            [&_f = failures]() -> int {
                                                ++_f;
                                                return 0;
                                            },
                                            _task_counter));

    sut = when_any(
        make_executor<1>(),
        [&_index = index](int x, size_t index) {
            _index = index;
            return x;
        },
        std::move(a1), std::move(a2), std::move(a3), std::move(a4));
    check_valid_future(sut);

    auto result = await(std::move(sut));
    wait_until_all_tasks_completed();

    BOOST_REQUIRE_EQUAL(size_t(2), index);
    BOOST_REQUIRE_EQUAL(3, result);
    BOOST_WARN_GE(3, failures.load());
    BOOST_REQUIRE_LE(4, custom_scheduler<0>::usage_counter());
    BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
}

/*
      /  F1  \
     / / F2 \ \
start           sut
     \ \ F3 / /
      \  F4  /
*/
BOOST_AUTO_TEST_CASE(future_when_any_int_arguments_with_diamond_formation_arguments) {
    BOOST_TEST_MESSAGE("running future when_any int arguments with diamond formation");

    thread_block_context block_context;

    size_t index = 0;
    {
        lock_t const lock(*block_context._mutex);
        auto start = async(make_executor<0>(), [] { return 4711; });

        auto a1 =
            start.then(make_executor<0>(), make_blocking_functor([](int x) { return x + 1; },
                                                                 _task_counter, block_context));
        auto a2 = start.then(make_executor<0>(), make_non_blocking_functor(
                                                     [&_context = block_context](auto x) {
                                                         _context._may_proceed = true;
                                                         return x + 2;
                                                     },
                                                     _task_counter));
        auto a3 =
            start.then(make_executor<0>(), make_blocking_functor([](int x) { return x + 3; },
                                                                 _task_counter, block_context));
        auto a4 =
            start.then(make_executor<0>(), make_blocking_functor([](int x) { return x + 5; },
                                                                 _task_counter, block_context));

        sut = when_any(
            make_executor<1>(),
            [&_i = index](int x, size_t index) {
                _i = index;
                return x;
            },
            std::move(a1), std::move(a2), std::move(a3), std::move(a4));

        check_valid_future(sut);
        wait_until_future_completed(copy(sut));
        block_context._go = true;
    }

    block_context._thread_block.notify_all();
    wait_until_all_tasks_completed();

    BOOST_REQUIRE_EQUAL(size_t(1), index);
    BOOST_REQUIRE_EQUAL(4711 + 2, *sut.get_try());
    BOOST_REQUIRE_LE(4, custom_scheduler<0>::usage_counter());
    BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(future_when_any_argument_move_only, test_fixture<move_only>)
BOOST_AUTO_TEST_CASE(future_when_any_move_only_argument_with_one_argument) {
    BOOST_TEST_MESSAGE("running future when_any move_only arguments of one argument");
    size_t index = 42;

    auto a1 = async(make_executor<0>(), [] { return move_only(4711); });

    sut = when_any(
        make_executor<1>(),
        [&_i = index](move_only(x), size_t index) {
            _i = index;
            return x;
        },
        std::move(a1));

    check_valid_future(sut);

    auto result = await(std::move(sut));

    BOOST_REQUIRE_EQUAL(size_t(0), index);
    BOOST_REQUIRE_EQUAL(4711, result.member());
    BOOST_REQUIRE_LE(1, custom_scheduler<0>::usage_counter());
    BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_SUITE_END()
