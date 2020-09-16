/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#include <boost/test/unit_test.hpp>

#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/future.hpp>
#include <stlab/concurrency/utility.hpp>
#include <stlab/test/model.hpp>

#include <array>

#include "future_test_helper.hpp"

using namespace stlab;
using namespace future_test_helper;

using lock_t = std::unique_lock<std::mutex>;

BOOST_FIXTURE_TEST_SUITE(future_when_any_range_void, test_fixture<void>)
BOOST_AUTO_TEST_CASE(future_when_any_void_void_empty_range) {
    BOOST_TEST_MESSAGE("running future when_any void void with empty range");
    bool check{false};
    std::vector<stlab::future<void>> emptyFutures;

    sut = when_any(make_executor<0>(), [& _check = check](size_t) { _check = true; },
                   std::make_pair(emptyFutures.begin(), emptyFutures.end()));

    wait_until_future_fails<stlab::future_error>(sut);

    BOOST_REQUIRE(!check);
    BOOST_REQUIRE_EQUAL(0, custom_scheduler<0>::usage_counter());
}

BOOST_AUTO_TEST_CASE(future_when_any_int_void_empty_range) {
    BOOST_TEST_MESSAGE("running future when_any int void with empty range");
    bool check{false};
    std::vector<stlab::future<int>> emptyFutures;

    sut = when_any(make_executor<0>(), [& _check = check](int, size_t) { _check = true; },
                   std::make_pair(emptyFutures.begin(), emptyFutures.end()));

    wait_until_future_fails<stlab::future_error>(sut);

    BOOST_REQUIRE(!check);
    BOOST_REQUIRE_EQUAL(0, custom_scheduler<0>::usage_counter());
}

BOOST_AUTO_TEST_CASE(future_when_any_void_void_range_with_one_element) {
    BOOST_TEST_MESSAGE("running future when_any void void with one element");

    int interim_result = 0;
    int result = 0;
    std::vector<stlab::future<void>> futures;
    futures.push_back(async(make_executor<0>(),
                            [& _interim_result = interim_result] { _interim_result = 42; }));

    sut = when_any(make_executor<1>(),
                   [& _result = result, &_interim_result = interim_result](size_t) {
                       _result = _interim_result;
                   },
                   std::make_pair(futures.begin(), futures.end()));

    wait_until_future_completed(sut);

    BOOST_REQUIRE_EQUAL(42, result);
    BOOST_REQUIRE_LE(1, custom_scheduler<0>::usage_counter());
    BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_CASE(future_when_any_int_void_range_with_one_element) {
    BOOST_TEST_MESSAGE("running future when_any int void with range of one element");
    size_t index = 4711;
    size_t result = 0;
    std::vector<stlab::future<int>> futures;
    futures.push_back(async(make_executor<0>(), [] { return 42; }));

    sut = when_any(make_executor<1>(),
                   [& _i = index, &_r = result](int x, size_t index) {
                       _i = index;
                       _r = x;
                   },
                   std::make_pair(futures.begin(), futures.end()));

    check_valid_future(sut);
    wait_until_future_completed(sut);

    BOOST_REQUIRE_EQUAL(size_t(0), index);
    BOOST_REQUIRE_EQUAL(size_t(42), result);
    BOOST_REQUIRE_LE(1, custom_scheduler<0>::usage_counter());
    BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_CASE(future_when_any_int_void_range_with_many_elements_first_succeeds) {
    BOOST_TEST_MESSAGE(
        "running future when_any int void with range with many elements and the first succeeds");

    thread_block_context block_context;
    std::atomic_int any_task_execution_counter{0};
    size_t used_future_index = 0;
    size_t result = 0;

    std::vector<stlab::future<int>> futures;
    futures.push_back(async(make_executor<0>(), make_non_blocking_functor(
                                                       [& _context = block_context]() {
                                                           _context._may_proceed = true;
                                                           return 1;
                                                       },
                                                       _task_counter)));
    futures.push_back(async(make_executor<0>(),
                            make_blocking_functor([] { return 2; }, _task_counter, block_context)));
    futures.push_back(async(make_executor<0>(),
                            make_blocking_functor([] { return 3; }, _task_counter, block_context)));
    futures.push_back(async(make_executor<0>(),
                            make_blocking_functor([] { return 5; }, _task_counter, block_context)));
    {
        lock_t block(*block_context._mutex);

        sut = when_any(make_executor<1>(),
                       [& _r = result, &_used_future_index = used_future_index,
                        &_counter = any_task_execution_counter](int x, size_t index) {
                           _used_future_index = index;
                           ++_counter;
                           _r = x;
                       },
                       std::make_pair(futures.begin(), futures.end()));
        check_valid_future(sut);
        wait_until_future_completed(sut);
        block_context._go = true;
    }

    block_context._thread_block.notify_all();
    wait_until_all_tasks_completed();

    BOOST_REQUIRE_EQUAL(size_t(0), used_future_index);
    BOOST_REQUIRE_EQUAL(size_t(1), result);
    BOOST_REQUIRE_EQUAL(1, any_task_execution_counter.load());
    BOOST_REQUIRE_LE(4, custom_scheduler<0>::usage_counter());
    BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_CASE(future_when_any_int_void_range_with_many_elements_middle_succeeds) {
    BOOST_TEST_MESSAGE(
        "running future when_any int void with range with many elements and one in the middle succeeds");

    thread_block_context block_context;
    std::atomic_int any_task_execution_counter{0};
    size_t used_future_index = 0;
    size_t result = 0;

    std::vector<stlab::future<int>> futures;
    futures.push_back(async(make_executor<0>(),
                            make_blocking_functor([] { return 1; }, _task_counter, block_context)));
    futures.push_back(async(make_executor<0>(),
                            make_blocking_functor([] { return 2; }, _task_counter, block_context)));
    futures.push_back(async(make_executor<0>(), make_non_blocking_functor(
                                                       [& _context = block_context] {
                                                           _context._may_proceed = true;
                                                           return 3;
                                                       },
                                                       _task_counter)));
    futures.push_back(async(make_executor<0>(),
                            make_blocking_functor([] { return 5; }, _task_counter, block_context)));

    {
        lock_t lock(*block_context._mutex);

        sut = when_any(make_executor<1>(),
                       [& _r = result, &_used_future_index = used_future_index,
                        &_counter = any_task_execution_counter](int x, size_t index) {
                           _used_future_index = index;
                           ++_counter;
                           _r = x;
                       },
                       std::make_pair(futures.begin(), futures.end()));

        check_valid_future(sut);
        wait_until_future_completed(sut);
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

BOOST_AUTO_TEST_CASE(future_when_any_int_void_range_with_many_elements_last_succeeds) {
    BOOST_TEST_MESSAGE(
        "running future when_any int void with range with many elements and the last succeeds");

    thread_block_context block_context;
    std::atomic_int any_task_execution_counter{0};
    size_t used_future_index = 0;
    size_t result = 0;

    std::vector<stlab::future<int>> futures;
    futures.push_back(async(make_executor<0>(),
                            make_blocking_functor([] { return 1; }, _task_counter, block_context)));
    futures.push_back(async(make_executor<0>(),
                            make_blocking_functor([] { return 2; }, _task_counter, block_context)));
    futures.push_back(async(make_executor<0>(),
                            make_blocking_functor([] { return 3; }, _task_counter, block_context)));
    futures.push_back(async(make_executor<0>(), make_non_blocking_functor(
                                                       [& _context = block_context] {
                                                           _context._may_proceed = true;
                                                           return 5;
                                                       },
                                                       _task_counter)));
    {
        lock_t lock(*block_context._mutex);

        sut = when_any(make_executor<1>(),
                       [& _r = result, &_used_future_index = used_future_index,
                        &_counter = any_task_execution_counter](int x, size_t index) {
                           _used_future_index = index;
                           ++_counter;
                           _r = x;
                       },
                       std::make_pair(futures.begin(), futures.end()));

        check_valid_future(sut);
        wait_until_future_completed(sut);
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
    future_when_any_int_void_range_with_many_elements_one_succeeds_all_other_fails) {
    BOOST_TEST_MESSAGE(
        "running future when_any int void with range with many elements and one succeeds all other fails");

    std::atomic_int any_task_execution_counter{0};
    size_t index = 4711;
    int result = 0;

    std::vector<stlab::future<int>> futures;
    futures.push_back(
        async(make_executor<0>(), make_failing_functor([] { return 1; }, _task_counter)));
    futures.push_back(
        async(make_executor<0>(), make_failing_functor([] { return 1; }, _task_counter)));
    futures.push_back(
        async(make_executor<0>(), make_non_blocking_functor([] { return 3; }, _task_counter)));
    futures.push_back(
        async(make_executor<0>(), make_failing_functor([] { return 1; }, _task_counter)));

    sut = when_any(make_executor<1>(),
                   [& _i = index, &_result = result,
                    &_counter = any_task_execution_counter](int x, size_t index) {
                       ++_counter;
                       _i = index;
                       _result = x;
                   },
                   std::make_pair(futures.begin(), futures.end()));

    check_valid_future(sut);
    wait_until_future_completed(sut);
    wait_until_all_tasks_completed();

    BOOST_REQUIRE_EQUAL(size_t(2), index);
    BOOST_REQUIRE_EQUAL(3, result);
    BOOST_REQUIRE_EQUAL(1, any_task_execution_counter.load());
    BOOST_REQUIRE_LE(4, custom_scheduler<0>::usage_counter());
    BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_CASE(future_when_any_void_all_are_ready_at_the_beginning) {
    BOOST_TEST_MESSAGE("running future when_any void when all futures are ready from the start");

    std::vector<stlab::future<void>> tasks{
        stlab::make_ready_future(stlab::default_executor),
        stlab::make_ready_future(stlab::default_executor),
    };

    bool check{false};

    sut = stlab::when_any(
        stlab::default_executor, [&check](auto) { check = true; },
        std::make_pair(std::begin(tasks), std::end(tasks)));

    check_valid_future(sut);
    wait_until_future_completed(sut);
    BOOST_REQUIRE(check);
}


BOOST_AUTO_TEST_CASE(future_when_any_int_void_range_with_many_elements_all_fails) {
    BOOST_TEST_MESSAGE("running future when_any int void with range all fails");
    std::atomic_size_t failures{0};
    size_t index = 4711;
    int r = 0;

    std::vector<stlab::future<int>> futures;
    futures.push_back(
        async(make_executor<0>(), make_failing_functor([] { return 0; }, _task_counter)));
    futures.push_back(
        async(make_executor<0>(), make_failing_functor([] { return 0; }, _task_counter)));
    futures.push_back(
        async(make_executor<0>(), make_failing_functor([] { return 0; }, _task_counter)));
    futures.push_back(
        async(make_executor<0>(), make_failing_functor([] { return 0; }, _task_counter)));

    sut = when_any(make_executor<1>(),
                   [& _i = index, &_r = r](int x, size_t index) {
                       _i = index;
                       _r = x;
                   },
                   std::make_pair(futures.begin(), futures.end()));

    wait_until_all_tasks_completed();
    wait_until_future_fails<test_exception>(sut);

    BOOST_REQUIRE_EQUAL(size_t(4711), index);
    BOOST_REQUIRE_EQUAL(0, r);
    BOOST_WARN_GE(size_t(4), failures.load());
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
BOOST_AUTO_TEST_CASE(future_when_any_void_range_with_diamond_formation_elements) {
    BOOST_TEST_MESSAGE("running future when_any void with range with diamond formation");

    thread_block_context block_context;
    std::atomic_int any_task_execution_counter{0};
    std::atomic_int result{0};
    std::array<int, 4> intrim_results = {{0, 0, 0, 0}};

    {
        lock_t block(*block_context._mutex);
        auto start = async(make_executor<0>(), [] { return 4711; });
        std::vector<future<void>> futures;
        futures.push_back(start.then(
            make_executor<0>(),
            make_blocking_functor([& _result = intrim_results](auto x) { _result[0] = x + 1; },
                                  _task_counter, block_context)));
        futures.push_back(start.then(
            make_executor<0>(),
            make_blocking_functor([& _result = intrim_results](auto x) { _result[1] = x + 2; },
                                  _task_counter, block_context)));
        futures.push_back(start.then(
            make_executor<0>(),
            make_blocking_functor([& _result = intrim_results](auto x) { _result[2] = x + 3; },
                                  _task_counter, block_context)));
        futures.push_back(
            start.then(make_executor<0>(),
                       make_non_blocking_functor(
                           [& _context = block_context, &_result = intrim_results](auto x) {
                               _result[3] = x;
                               _context._may_proceed = true;
                           },
                           _task_counter)));

        sut = when_any(make_executor<1>(),
                       [& _result = result, &_counter = any_task_execution_counter,
                        &_interim_results = intrim_results](size_t index) {
                           _result = _interim_results[index];
                           ++_counter;
                       },
                       std::make_pair(futures.begin(), futures.end()));

        check_valid_future(sut);
        wait_until_future_completed(sut);
        block_context._go = true;
    }
    block_context._thread_block.notify_all();
    wait_until_all_tasks_completed();

    BOOST_REQUIRE_EQUAL(4711, result.load());
    BOOST_REQUIRE_EQUAL(1, any_task_execution_counter.load());
    BOOST_REQUIRE_LE(4, custom_scheduler<0>::usage_counter());
    BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(future_when_any_range_int, test_fixture<int>)
BOOST_AUTO_TEST_CASE(future_when_any_int_int_empty_range) {
    BOOST_TEST_MESSAGE("running future when_any int int with empty range");

    bool check{false};
    std::vector<stlab::future<int>> emptyFutures;
    sut = when_any(make_executor<0>(),
                   [& _check = check](int x, size_t) {
                       _check = true;
                       return x + 42;
                   },
                   std::make_pair(emptyFutures.begin(), emptyFutures.end()));

    wait_until_future_fails<stlab::future_error>(sut);

    BOOST_REQUIRE(!check);
    BOOST_REQUIRE_EQUAL(0, custom_scheduler<0>::usage_counter());
}

BOOST_AUTO_TEST_CASE(future_when_any_int_int_range_with_one_element) {
    BOOST_TEST_MESSAGE("running future when_any int int with range of one element");
    size_t index = 42;

    std::vector<stlab::future<int>> futures;
    futures.push_back(async(make_executor<0>(), [] { return 4711; }));

    sut = when_any(make_executor<1>(),
                   [& _i = index](int x, size_t index) {
                       _i = index;
                       return x;
                   },
                   std::make_pair(futures.begin(), futures.end()));
    check_valid_future(sut);

    wait_until_future_completed(sut);

    BOOST_REQUIRE_EQUAL(size_t(0), index);
    BOOST_REQUIRE_EQUAL(4711, *sut.get_try());
    BOOST_REQUIRE_LE(1, custom_scheduler<0>::usage_counter());
    BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());

}

BOOST_AUTO_TEST_CASE(future_when_any_int_int_range_with_many_elements) {
    BOOST_TEST_MESSAGE(
        "running future when_any int int with range with many elements and the last suceeds");

    thread_block_context block_context;

    std::atomic_int any_task_execution_counter{0};
    size_t used_future_index = 0;

    std::vector<stlab::future<int>> futures;
    futures.push_back(
        async(make_executor<0>(),
              make_blocking_functor([] { return 42; }, _task_counter, block_context)));
    futures.push_back(
        async(make_executor<0>(),
              make_blocking_functor([] { return 815; }, _task_counter, block_context)));
    futures.push_back(
        async(make_executor<0>(),
              make_blocking_functor([] { return 4711; }, _task_counter, block_context)));
    futures.push_back(async(make_executor<0>(), make_non_blocking_functor(
                                                       [& _context = block_context] {
                                                           _context._may_proceed = true;
                                                           return 5;
                                                       },
                                                       _task_counter)));

    {
        lock_t lock(*block_context._mutex);
        sut = when_any(make_executor<1>(),
                       [& _used_future_index = used_future_index,
                        &_counter = any_task_execution_counter](int x, size_t index) {
                           _used_future_index = index;
                           ++_counter;
                           return x;
                       },
                       std::make_pair(futures.begin(), futures.end()));
        check_valid_future(sut);
        wait_until_future_completed(sut);
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

BOOST_AUTO_TEST_CASE(future_when_any_int_int_range_with_many_elements_all_but_all_one_failing) {
    BOOST_TEST_MESSAGE(
        "running future when_any int int range with many elements all but one is failing");
    size_t index = 0;
    std::atomic_int failures{0};
    std::vector<stlab::future<int>> futures;
    futures.push_back(async(make_executor<0>(), make_failing_functor(
                                                       [& _f = failures]() -> int {
                                                           ++_f;
                                                           return 0;
                                                       },
                                                       _task_counter)));
    futures.push_back(async(make_executor<0>(), make_failing_functor(
                                                       [& _f = failures]() -> int {
                                                           ++_f;
                                                           return 0;
                                                       },
                                                       _task_counter)));
    futures.push_back(async(make_executor<0>(),
                            make_non_blocking_functor([]() -> int { return 3; }, _task_counter)));
    futures.push_back(async(make_executor<0>(), make_failing_functor(
                                                       [& _f = failures]() -> int {
                                                           ++_f;
                                                           return 0;
                                                       },
                                                       _task_counter)));

    sut = when_any(make_executor<1>(),
                   [& _index = index](int x, size_t index) {
                       _index = index;
                       return x;
                   },
                   std::make_pair(futures.begin(), futures.end()));
    check_valid_future(sut);

    wait_until_future_completed(sut);
    wait_until_all_tasks_completed();

    BOOST_REQUIRE_EQUAL(size_t(2), index);
    BOOST_REQUIRE_EQUAL(3, *sut.get_try());
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
BOOST_AUTO_TEST_CASE(future_when_any_int_range_with_diamond_formation_elements) {
    BOOST_TEST_MESSAGE("running future when_any int with range with diamond formation");

    thread_block_context block_context;

    size_t index = 0;
    {
        lock_t lock(*block_context._mutex);
        auto start = async(make_executor<0>(), [] { return 4711; });
        std::vector<stlab::future<int>> futures;
        futures.push_back(
            start.then(make_executor<0>(), make_blocking_functor([](int x) { return x + 1; },
                                                                    _task_counter, block_context)));
        futures.push_back(
            start.then(make_executor<0>(), make_non_blocking_functor(
                                                  [& _context = block_context](auto x) {
                                                      _context._may_proceed = true;
                                                      return x + 2;
                                                  },
                                                  _task_counter)));
        futures.push_back(
            start.then(make_executor<0>(), make_blocking_functor([](int x) { return x + 3; },
                                                                    _task_counter, block_context)));
        futures.push_back(
            start.then(make_executor<0>(), make_blocking_functor([](int x) { return x + 5; },
                                                                    _task_counter, block_context)));

        sut = when_any(make_executor<1>(),
                       [& _i = index](int x, size_t index) {
                           _i = index;
                           return x;
                       },
                       std::make_pair(futures.begin(), futures.end()));

        check_valid_future(sut);
        wait_until_future_completed(sut);
        block_context._go = true;
    }

    block_context._thread_block.notify_all();
    wait_until_all_tasks_completed();

    BOOST_REQUIRE_EQUAL(size_t(1), index);
    BOOST_REQUIRE_EQUAL(4711 + 2, *sut.get_try());
    BOOST_REQUIRE_LE(2, custom_scheduler<0>::usage_counter());
    BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
}
BOOST_AUTO_TEST_SUITE_END()



BOOST_FIXTURE_TEST_SUITE(future_when_any_range_move_only, test_fixture<stlab::move_only>)

BOOST_AUTO_TEST_CASE(future_when_any_move_only_range_with_diamond_formation_elements) {
  BOOST_TEST_MESSAGE("running future when_any move_only with range with diamond formation");

  thread_block_context block_context;

  size_t index = 0;
  {
    lock_t lock(*block_context._mutex);
    auto start = async(make_executor<0>(), [] { return 4711; });
    std::vector<stlab::future<stlab::move_only>> futures;
    futures.push_back(
      start.then(make_executor<0>(), make_blocking_functor([](int x) { return stlab::move_only{ x + 1 }; },
        _task_counter, block_context)));
    futures.push_back(
      start.then(make_executor<0>(), make_non_blocking_functor(
        [&_context = block_context](auto x) {
      _context._may_proceed = true;
      return stlab::move_only{ x + 2 };
    },
        _task_counter)));
    futures.push_back(
      start.then(make_executor<0>(), make_blocking_functor([](int x) { return stlab::move_only{ x + 3 }; },
        _task_counter, block_context)));
    futures.push_back(
      start.then(make_executor<0>(), make_blocking_functor([](int x) { return stlab::move_only{ x + 5 }; },
        _task_counter, block_context)));

    sut = when_any(make_executor<1>(),
      [&_i = index](stlab::move_only x, size_t index) {
      _i = index;
      return x;
    },
      std::make_pair(futures.begin(), futures.end()));

    check_valid_future(sut);
    wait_until_future_completed(sut);
    block_context._go = true;
  }

  block_context._thread_block.notify_all();
  wait_until_all_tasks_completed();

  BOOST_REQUIRE_EQUAL(size_t(1), index);
  BOOST_REQUIRE_EQUAL(4711 + 2, (*sut.get_try()).member());
  BOOST_REQUIRE_LE(2, custom_scheduler<0>::usage_counter());
  BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_SUITE_END()