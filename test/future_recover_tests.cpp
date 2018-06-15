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

#include "future_test_helper.hpp"

using namespace stlab;
using namespace future_test_helper;

BOOST_FIXTURE_TEST_SUITE(future_recover_void, test_fixture<void>)
BOOST_AUTO_TEST_CASE(future_recover_failure_before_recover_initialized_on_rvalue) {
    BOOST_TEST_MESSAGE("running future recover, failure before recover initialized on r-value");

    auto error = false;

    sut = async(custom_scheduler<0>(),
                [& _error = error] {
                    _error = true;
                    throw test_exception("failure");
                })
              .recover([](auto failedFuture) {
                  if (failedFuture.error()) check_failure<test_exception>(failedFuture, "failure");
              });
    wait_until_future_completed(sut);

    BOOST_REQUIRE(error);
    BOOST_REQUIRE_GE(2, custom_scheduler<0>::usage_counter());
}

BOOST_AUTO_TEST_CASE(future_recover_failure_before_recover_initialized_on_lvalue) {
    BOOST_TEST_MESSAGE("running future recover, failure before recover initialized on l-value");

    auto error = false;
    auto interim = async(custom_scheduler<0>(), [& _error = error] {
        _error = true;
        throw test_exception("failure");
    });

    wait_until_future_fails<test_exception>(interim);

    sut = interim.recover(
        [](auto failedFuture) { check_failure<test_exception>(failedFuture, "failure"); });

    wait_until_future_completed(sut);

    BOOST_REQUIRE(error);
    BOOST_REQUIRE_EQUAL(2, custom_scheduler<0>::usage_counter());
}

BOOST_AUTO_TEST_CASE(
    future_recover_failure_before_recover_initialized_with_custom_scheduler_on_rvalue) {
    BOOST_TEST_MESSAGE(
        "running future recover, failure before recover initialized, with custom scheduler on r-value");

    auto error = false;

    sut = async(custom_scheduler<0>(),
                [& _error = error] {
                    _error = true;
                    throw test_exception("failure");
                })
              .recover(custom_scheduler<1>(), [](auto failedFuture) {
                  check_failure<test_exception>(failedFuture, "failure");
              });

    wait_until_future_completed(sut);

    BOOST_REQUIRE(error);
    BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
    BOOST_REQUIRE_GE(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_CASE(
    future_recover_failure_before_recover_initialized_with_custom_scheduler_on_lvalue) {
    BOOST_TEST_MESSAGE(
        "running future recover, failure before recover initialized, with custom scheduler on l-value");

    auto error = false;
    auto interim = async(custom_scheduler<0>(), [& _error = error] {
        _error = true;
        throw test_exception("failure");
    });

    wait_until_future_fails<test_exception>(interim);

    sut = interim.recover(custom_scheduler<1>(), [](auto failedFuture) {
        check_failure<test_exception>(failedFuture, "failure");
    });

    wait_until_future_completed(sut);

    BOOST_REQUIRE(error);
    BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
    BOOST_REQUIRE_EQUAL(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_CASE(future_recover_failure_after_recover_initialized_on_rvalue) {
    BOOST_TEST_MESSAGE("running future recover, failure after recover initialized on r-value");

    auto error = false;
    std::mutex block;

    {
        lock_t hold(block);
        sut = async(custom_scheduler<0>(),
                    [& _error = error, &_block = block] {
                        lock_t lock(_block);
                        _error = true;
                        throw test_exception("failure");
                    })
                  .recover([](auto failedFuture) {
                      check_failure<test_exception>(failedFuture, "failure");
                  });
    }

    wait_until_future_completed(sut);

    BOOST_REQUIRE(error);
    BOOST_REQUIRE_GE(2, custom_scheduler<0>::usage_counter());
}

BOOST_AUTO_TEST_CASE(future_recover_failure_after_recover_initialized_on_lvalue) {
    BOOST_TEST_MESSAGE("running future recover, failure after recover initialized on l-value");

    auto error = false;
    std::mutex block;

    {
        lock_t hold(block);
        auto interim = async(custom_scheduler<0>(), [& _error = error, &_block = block] {
            lock_t lock(_block);
            _error = true;
            throw test_exception("failure");
        });

        sut = interim.recover(
            [](auto failedFuture) { check_failure<test_exception>(failedFuture, "failure"); });
    }

    wait_until_future_completed(sut);

    BOOST_REQUIRE(error);
    BOOST_REQUIRE_GE(2, custom_scheduler<0>::usage_counter());
}

BOOST_AUTO_TEST_CASE(
    future_recover_failure_after_recover_initialized_with_custom_scheduler_on_rvalue) {
    BOOST_TEST_MESSAGE(
        "running future recover, failure after recover initialized with custom scheduler on r-value");

    auto error = false;
    std::mutex block;

    {
        lock_t hold(block);
        sut = async(custom_scheduler<0>(),
                    [& _error = error, &_block = block] {
                        lock_t lock(_block);
                        _error = true;
                        throw test_exception("failure");
                    })
                  .recover(custom_scheduler<1>(), [](auto failedFuture) {
                      check_failure<test_exception>(failedFuture, "failure");
                  });
    }

    wait_until_future_completed(sut);

    BOOST_REQUIRE(error);
    BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
    BOOST_REQUIRE_GE(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_CASE(
    future_recover_failure_after_recover_initialized_with_custom_scheduler_on_lvalue) {
    BOOST_TEST_MESSAGE(
        "running future recover, failure after recover initialized with custom scheduler on l-value");

    auto error = false;
    std::mutex block;

    {
        lock_t hold(block);
        auto interim = async(custom_scheduler<0>(), [& _error = error, &_block = block] {
            lock_t lock(_block);
            _error = true;
            throw test_exception("failure");
        });

        sut = interim.recover(custom_scheduler<1>(), [](auto failedFuture) {
            check_failure<test_exception>(failedFuture, "failure");
        });
    }

    wait_until_future_completed(sut);

    BOOST_REQUIRE(error);
    BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
    BOOST_REQUIRE_GE(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_CASE(future_recover_failure_during_when_all_on_lvalue) {
    BOOST_TEST_MESSAGE("running future recover while failed when_all on l-value");

    int result{0};
    auto f1 = async(custom_scheduler<0>(), []() -> int { throw test_exception("failure"); });
    auto f2 = async(custom_scheduler<1>(), [] { return 42; });

    sut = when_all(custom_scheduler<0>(), [](int x, int y) { return x + y; }, f1, f2)
              .recover([](auto error) {
                  if (error.error())
                      return 815;
                  else
                      return 0;
              })
              .then([&](int x) { result = x; });

    wait_until_future_completed(sut);
    BOOST_REQUIRE_EQUAL(815, result);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(future_recover_int, test_fixture<int>)
BOOST_AUTO_TEST_CASE(
    future_recover_int_simple_recover_failure_before_recover_initialized_on_rvalue) {
    BOOST_TEST_MESSAGE("running future int recover, failure before recover initialized on r-value");

    auto error = false;

    sut = async(custom_scheduler<0>(),
                [& _error = error]() -> int {
                    _error = true;
                    throw test_exception("failure");
                })
              .recover([](auto failedFuture) {
                  check_failure<test_exception>(failedFuture, "failure");
                  return 42;
              });

    wait_until_future_completed(sut);

    BOOST_REQUIRE_EQUAL(42, *sut.get_try());
    BOOST_REQUIRE(error);
}

BOOST_AUTO_TEST_CASE(
    future_recover_int_simple_recover_failure_before_recover_initialized_on_lvalue) {
    BOOST_TEST_MESSAGE("running future int recover, failure before recover initialized on l-value");

    auto error = false;
    auto interim = async(custom_scheduler<0>(), [& _error = error]() -> int {
        _error = true;
        throw test_exception("failure");
    });

    sut = interim.recover([](auto failedFuture) {
        check_failure<test_exception>(failedFuture, "failure");
        return 42;
    });

    wait_until_future_completed(sut);

    BOOST_REQUIRE_EQUAL(42, *sut.get_try());
    BOOST_REQUIRE(error);
}

BOOST_AUTO_TEST_CASE(
    future_recover_int_simple_recover_failure_after_recover_initialized_on_rvalue) {
    BOOST_TEST_MESSAGE("running future int recover, failure after recover initialized on r-value");

    auto error = false;
    std::mutex block;

    {
        lock_t hold(block);

        sut = async(custom_scheduler<0>(),
                    [& _error = error, &_block = block]() -> int {
                        lock_t lock(_block);
                        _error = true;
                        throw test_exception("failure");
                    })
                  .recover([](auto failedFuture) {
                      check_failure<test_exception>(failedFuture, "failure");
                      return 42;
                  });
    }

    wait_until_future_completed(sut);

    BOOST_REQUIRE_EQUAL(42, *sut.get_try());
    BOOST_REQUIRE(error);
}

BOOST_AUTO_TEST_CASE(
    future_recover_int_simple_recover_failure_after_recover_initialized_on_lvalue) {
    BOOST_TEST_MESSAGE("running future int recover, failure after recover initialized on l-value");

    auto error = false;
    std::mutex block;

    {
        lock_t hold(block);

        auto interim = async(custom_scheduler<0>(), [& _error = error, &_block = block]() -> int {
            lock_t lock(_block);
            _error = true;
            throw test_exception("failure");
        });

        sut = interim.recover([](auto failedFuture) {
            check_failure<test_exception>(failedFuture, "failure");
            return 42;
        });
    }

    wait_until_future_completed(sut);

    BOOST_REQUIRE_EQUAL(42, *sut.get_try());
    BOOST_REQUIRE(error);
}

BOOST_AUTO_TEST_CASE(
    future_recover_int_simple_recover_failure_before_recover_initialized_with_custom_scheduler_on_rvalue) {
    BOOST_TEST_MESSAGE(
        "running future int recover, failure before recover initialized with custom scheduler on r-value");

    auto error = false;

    sut = async(custom_scheduler<0>(),
                [& _error = error]() -> int {
                    _error = true;
                    throw test_exception("failure");
                })
              .recover(custom_scheduler<1>(), [](auto failedFuture) {
                  check_failure<test_exception>(failedFuture, "failure");
                  return 42;
              });

    wait_until_future_completed(sut);

    BOOST_REQUIRE_EQUAL(42, *sut.get_try());
    BOOST_REQUIRE(error);
    BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
    BOOST_REQUIRE_GE(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_CASE(
    future_recover_int_simple_recover_failure_before_recover_initialized_with_custom_scheduler_on_lvalue) {
    BOOST_TEST_MESSAGE(
        "running future int recover, failure before recover initialized with custom scheduler on l-value");

    auto error = false;
    auto interim = async(custom_scheduler<0>(), [& _error = error]() -> int {
        _error = true;
        throw test_exception("failure");
    });

    wait_until_future_fails<test_exception>(interim);

    sut = interim.recover(custom_scheduler<1>(), [](auto failedFuture) {
        check_failure<test_exception>(failedFuture, "failure");
        return 42;
    });

    wait_until_future_completed(sut);

    BOOST_REQUIRE_EQUAL(42, *sut.get_try());
    BOOST_REQUIRE(error);
    BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
    BOOST_REQUIRE_EQUAL(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_CASE(
    future_recover_int_simple_recover_failure_after_recover_initialized_with_custom_scheduler_on_rvalue) {
    BOOST_TEST_MESSAGE(
        "running future int recover, failure after recover initialized with custom scheduler on r-value");

    auto error = false;
    std::mutex block;

    {
        lock_t hold(block);

        sut = async(custom_scheduler<0>(),
                    [& _error = error, &_block = block]() -> int {
                        lock_t lock(_block);
                        _error = true;
                        throw test_exception("failure");
                    })
                  .recover(custom_scheduler<1>(), [](auto failedFuture) {
                      check_failure<test_exception>(failedFuture, "failure");
                      return 42;
                  });
    }

    wait_until_future_completed(sut);

    BOOST_REQUIRE_EQUAL(42, *sut.get_try());
    BOOST_REQUIRE(error);
    BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
    BOOST_REQUIRE_GE(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_CASE(
    future_recover_int_simple_recover_failure_after_recover_initialized_with_custom_scheduler_on_lvalue) {
    BOOST_TEST_MESSAGE(
        "running future int recover, failure after recover initialized with custom scheduler on l-value");

    auto error = false;
    std::mutex block;

    {
        lock_t hold(block);
        auto interim = async(custom_scheduler<0>(), [& _error = error, &_block = block]() -> int {
            lock_t lock(_block);
            _error = true;
            throw test_exception("failure");
        });

        sut = interim.recover(custom_scheduler<1>(), [](auto failedFuture) {
            check_failure<test_exception>(failedFuture, "failure");
            return 42;
        });
    }

    wait_until_future_completed(sut);

    BOOST_REQUIRE_EQUAL(42, *sut.get_try());
    BOOST_REQUIRE(error);
    BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
    BOOST_REQUIRE_GE(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(future_recover_move_only_type, test_fixture<stlab::move_only>)
BOOST_AUTO_TEST_CASE(
    future_recover_move_only_type_recover_failure_before_recover_initialized_on_rvalue) {
    BOOST_TEST_MESSAGE(
        "running future move only type recover, failure before recover initialized on r-value");

    auto error = false;

    sut = async(custom_scheduler<0>(),
                [& _error = error]() -> move_only {
                    _error = true;
                    throw test_exception("failure");
                })
              .recover([](auto failedFuture) {
                  check_failure<test_exception>(failedFuture, "failure");
                  return move_only(42);
              });

    auto result = wait_until_future_r_completed(sut);

    BOOST_REQUIRE_EQUAL(42, result->member());
    BOOST_REQUIRE(error);
}

BOOST_AUTO_TEST_CASE(future_recover_move_only_types_recover_failure_after_recover_initialized) {
    BOOST_TEST_MESSAGE("running future move only type recover, failure after recover initialized");

    auto error = false;
    std::mutex block;
    {
        lock_t hold(block);

        sut = async(custom_scheduler<0>(),
                    [& _error = error, &_block = block]() -> move_only {
                        lock_t lock(_block);
                        _error = true;
                        throw test_exception("failure");
                    })
                  .recover([](auto failedFuture) {
                      check_failure<test_exception>(failedFuture, "failure");
                      return move_only(42);
                  });
    }

    auto result = wait_until_future_r_completed(sut);

    BOOST_REQUIRE_EQUAL(42, result->member());
    BOOST_REQUIRE(error);
}

BOOST_AUTO_TEST_CASE(
    future_recover_move_only_type_recover_failure_before_recover_initialized_with_custom_scheduler_on_rvalue) {
    BOOST_TEST_MESSAGE(
        "running future move only type recover, failure before recover initialized with custom scheduler on r-value");

    auto error = false;

    sut = async(custom_scheduler<0>(),
                [& _error = error]() -> move_only {
                    _error = true;
                    throw test_exception("failure");
                })
              .recover(custom_scheduler<1>(), [](auto failedFuture) {
                  check_failure<test_exception>(failedFuture, "failure");
                  return move_only(42);
              });

    auto result = wait_until_future_r_completed(sut);

    BOOST_REQUIRE_EQUAL(42, result->member());
    BOOST_REQUIRE(error);
    BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
    BOOST_REQUIRE_GE(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_CASE(
    future_recover_move_only_types_recover_failure_after_recover_initialized_with_custom_scheduler_on_rvalue) {
    BOOST_TEST_MESSAGE(
        "running future move only type recover, failure after recover initialized with custom scheduler on r-value");

    auto error = false;
    std::mutex block;
    {
        lock_t hold(block);
        sut = async(custom_scheduler<0>(),
                    [& _error = error, &_block = block]() -> move_only {
                        lock_t lock(_block);
                        _error = true;
                        throw test_exception("failure");
                    })
                  .recover(custom_scheduler<1>(), [](auto failedFuture) {
                      check_failure<test_exception>(failedFuture, "failure");
                      return move_only(42);
                  });
    }

    auto result = wait_until_future_r_completed(sut);

    BOOST_REQUIRE_EQUAL(42, result->member());
    BOOST_REQUIRE(error);
    BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
    BOOST_REQUIRE_GE(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_SUITE_END()
