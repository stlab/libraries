/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#include <exception>
#include <mutex>
#include <utility>

#include <boost/test/unit_test.hpp>

#include <stlab/concurrency/await.hpp>
#include <stlab/concurrency/executor_base.hpp>
#include <stlab/concurrency/future.hpp>
#include <stlab/concurrency/immediate_executor.hpp>
#include <stlab/test/model.hpp>
#include <stlab/utility.hpp>

#include "future_test_helper.hpp"

using namespace std;
using namespace stlab;
using namespace future_test_helper;

// ----------------------------------------------------------------------------
//                                  void
// ----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_SUITE(future_recover_void, test_fixture<void>)
BOOST_AUTO_TEST_CASE(future_recover_failure_before_recover_initialized_on_rvalue) {
    BOOST_TEST_MESSAGE("running future recover, failure before recover initialized on r-value");

    /*
    combining the tests as in future_then_tests is not possible because of a bug in gcc

    using task_t = function<void(future<void>)>;
    using op_t = future<void>(future<void>::*)(task_t&&)&&;

    op_t ops[] = {static_cast<op_t>(&future<void>::recover<task_t>),
                  static_cast<op_t>(&future<void>::operator^<task_t>)};

    for (const auto& op : ops)
    */
    {
        custom_scheduler<0>::reset();
        auto error = false;
        sut = async(make_executor<0>(), [&_error = error]() -> void {
                  _error = true;
                  throw test_exception("failure");
              }).recover([](future<void> failedFuture) {
            if (failedFuture.exception()) {
                check_failure<test_exception>(failedFuture, "failure");
            }
        });
        wait_until_future_completed(std::move(sut));

        BOOST_REQUIRE(error);
        BOOST_REQUIRE_GE(2, custom_scheduler<0>::usage_counter());
    }
    {
        custom_scheduler<0>::reset();
        auto error = false;
        sut = (async(make_executor<0>(),
                     [&_error = error]() -> void {
                         _error = true;
                         throw test_exception("failure");
                     }) ^
               [](future<void> failedFuture) {
                   if (failedFuture.exception()) {
                       check_failure<test_exception>(failedFuture, "failure");
                   }
               });
        wait_until_future_completed(std::move(sut));

        BOOST_REQUIRE(error);
        BOOST_REQUIRE_GE(2, custom_scheduler<0>::usage_counter());
    }
}

BOOST_AUTO_TEST_CASE(future_recover_failure_before_recover_initialized_on_lvalue) {
    BOOST_TEST_MESSAGE("running future recover, failure before recover initialized on l-value");

    {
        custom_scheduler<0>::reset();
        auto error = false;
        auto interim = async(make_executor<0>(), [&_error = error] {
            _error = true;
            throw test_exception("failure");
        });

        wait_until_future_fails<test_exception>(copy(interim));

        sut = interim.recover(
            [](auto failedFuture) { check_failure<test_exception>(failedFuture, "failure"); });

        wait_until_future_completed(std::move(sut));

        BOOST_REQUIRE(error);
        BOOST_REQUIRE_EQUAL(2, custom_scheduler<0>::usage_counter());
    }
    {
        custom_scheduler<0>::reset();
        auto error = false;
        auto interim = async(make_executor<0>(), [&_error = error] {
            _error = true;
            throw test_exception("failure");
        });

        wait_until_future_fails<test_exception>(copy(interim));

        sut = interim ^
              [](auto failedFuture) { check_failure<test_exception>(failedFuture, "failure"); };

        wait_until_future_completed(std::move(sut));

        BOOST_REQUIRE(error);
        BOOST_REQUIRE_EQUAL(2, custom_scheduler<0>::usage_counter());
    }
}

BOOST_AUTO_TEST_CASE(
    future_recover_failure_before_recover_initialized_with_custom_scheduler_on_rvalue) {
    BOOST_TEST_MESSAGE(
        "running future recover, failure before recover initialized, with custom scheduler on r-value");

    {
        auto error = false;

        sut = async(make_executor<0>(), [&_error = error] {
                  _error = true;
                  throw test_exception("failure");
              }).recover(make_executor<1>(), [](auto failedFuture) {
            check_failure<test_exception>(failedFuture, "failure");
        });

        wait_until_future_completed(std::move(sut));

        BOOST_REQUIRE(error);
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_GE(1, custom_scheduler<1>::usage_counter());
    }

    custom_scheduler<0>::reset();
    custom_scheduler<1>::reset();

    {
        auto error = false;

        sut = async(make_executor<0>(),
                    [&_error = error] {
                        _error = true;
                        throw test_exception("failure");
                    }) ^
              (executor{make_executor<1>()} &
               [](auto failedFuture) { check_failure<test_exception>(failedFuture, "failure"); });

        wait_until_future_completed(std::move(sut));

        BOOST_REQUIRE(error);
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_GE(1, custom_scheduler<1>::usage_counter());
    }
}

BOOST_AUTO_TEST_CASE(
    future_recover_failure_before_recover_initialized_with_custom_scheduler_on_lvalue) {
    BOOST_TEST_MESSAGE(
        "running future recover, failure before recover initialized, with custom scheduler on l-value");

    {
        auto error = false;
        auto interim = async(make_executor<0>(), [&_error = error] {
            _error = true;
            throw test_exception("failure");
        });

        wait_until_future_fails<test_exception>(copy(interim));

        sut = interim.recover(make_executor<1>(), [](auto failedFuture) {
            check_failure<test_exception>(failedFuture, "failure");
        });

        wait_until_future_completed(std::move(sut));

        BOOST_REQUIRE(error);
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<1>::usage_counter());
    }

    custom_scheduler<0>::reset();
    custom_scheduler<1>::reset();

    {
        auto error = false;
        auto interim = async(make_executor<0>(), [&_error = error] {
            _error = true;
            throw test_exception("failure");
        });

        wait_until_future_fails<test_exception>(copy(interim));

        sut = interim ^ (executor{make_executor<1>()} & [](auto failedFuture) {
                  check_failure<test_exception>(failedFuture, "failure");
              });

        wait_until_future_completed(std::move(sut));

        BOOST_REQUIRE(error);
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<1>::usage_counter());
    }
}

BOOST_AUTO_TEST_CASE(future_recover_failure_after_recover_initialized_on_rvalue) {
    BOOST_TEST_MESSAGE("running future recover, failure after recover initialized on r-value");

    {
        custom_scheduler<0>::reset();
        auto error = false;
        mutex block;

        {
            lock_t const hold(block);
            sut = async(make_executor<0>(), [&_error = error, &_block = block] {
                      lock_t const lock(_block);
                      _error = true;
                      throw test_exception("failure");
                  }).recover([](auto failedFuture) {
                check_failure<test_exception>(failedFuture, "failure");
            });
        }

        wait_until_future_completed(std::move(sut));

        BOOST_REQUIRE(error);
        BOOST_REQUIRE_GE(2, custom_scheduler<0>::usage_counter());
    }
    {
        custom_scheduler<0>::reset();
        auto error = false;
        auto interim = async(make_executor<0>(), [&_error = error] {
            _error = true;
            throw test_exception("failure");
        });

        wait_until_future_fails<test_exception>(copy(interim));

        sut = interim ^
              [](auto failedFuture) { check_failure<test_exception>(failedFuture, "failure"); };

        wait_until_future_completed(std::move(sut));

        BOOST_REQUIRE(error);
        BOOST_REQUIRE_EQUAL(2, custom_scheduler<0>::usage_counter());
    }
}

BOOST_AUTO_TEST_CASE(future_recover_failure_after_recover_initialized_on_lvalue) {
    BOOST_TEST_MESSAGE("running future recover, failure after recover initialized on l-value");

    {
        custom_scheduler<0>::reset();
        auto error = false;
        mutex block;

        {
            lock_t const hold(block);
            auto interim = async(make_executor<0>(), [&_error = error, &_block = block] {
                lock_t const lock(_block);
                _error = true;
                throw test_exception("failure");
            });

            sut = interim.recover(
                [](auto failedFuture) { check_failure<test_exception>(failedFuture, "failure"); });
        }

        wait_until_future_completed(std::move(sut));

        BOOST_REQUIRE(error);
        BOOST_REQUIRE_GE(2, custom_scheduler<0>::usage_counter());
    }
    {
        custom_scheduler<0>::reset();
        auto error = false;
        mutex block;

        {
            lock_t const hold(block);
            auto interim = async(make_executor<0>(), [&_error = error, &_block = block] {
                lock_t const lock(_block);
                _error = true;
                throw test_exception("failure");
            });

            sut = interim ^
                  [](auto failedFuture) { check_failure<test_exception>(failedFuture, "failure"); };
        }

        wait_until_future_completed(std::move(sut));

        BOOST_REQUIRE(error);
        BOOST_REQUIRE_GE(2, custom_scheduler<0>::usage_counter());
    }
}

BOOST_AUTO_TEST_CASE(
    future_recover_failure_after_recover_initialized_with_custom_scheduler_on_rvalue) {
    BOOST_TEST_MESSAGE(
        "running future recover, failure after recover initialized with custom scheduler on r-value");

    {
        auto error = false;
        mutex block;

        {
            lock_t const hold(block);
            sut = async(make_executor<0>(), [&_error = error, &_block = block] {
                      lock_t const lock(_block);
                      _error = true;
                      throw test_exception("failure");
                  }).recover(make_executor<1>(), [](auto failedFuture) {
                check_failure<test_exception>(failedFuture, "failure");
            });
        }

        wait_until_future_completed(std::move(sut));

        BOOST_REQUIRE(error);
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_GE(1, custom_scheduler<1>::usage_counter());
    }

    custom_scheduler<0>::reset();
    custom_scheduler<1>::reset();

    {
        auto error = false;
        mutex block;

        {
            lock_t const hold(block);
            sut = async(make_executor<0>(),
                        [&_error = error, &_block = block] {
                            lock_t const lock(_block);
                            _error = true;
                            throw test_exception("failure");
                        }) ^
                  (executor{make_executor<1>()} & [](auto failedFuture) {
                      check_failure<test_exception>(failedFuture, "failure");
                  });
        }

        wait_until_future_completed(std::move(sut));

        BOOST_REQUIRE(error);
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_GE(1, custom_scheduler<1>::usage_counter());
    }
}

BOOST_AUTO_TEST_CASE(
    future_recover_failure_after_recover_initialized_with_custom_scheduler_on_lvalue) {
    BOOST_TEST_MESSAGE(
        "running future recover, failure after recover initialized with custom scheduler on l-value");

    {
        auto error = false;
        mutex block;

        {
            lock_t const hold(block);
            auto interim = async(make_executor<0>(), [&_error = error, &_block = block] {
                lock_t const lock(_block);
                _error = true;
                throw test_exception("failure");
            });

            sut = interim.recover(make_executor<1>(), [](auto failedFuture) {
                check_failure<test_exception>(failedFuture, "failure");
            });
        }

        wait_until_future_completed(std::move(sut));

        BOOST_REQUIRE(error);
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_GE(1, custom_scheduler<1>::usage_counter());
    }

    custom_scheduler<0>::reset();
    custom_scheduler<1>::reset();

    {
        auto error = false;
        mutex block;

        {
            lock_t const hold(block);
            auto interim = async(make_executor<0>(), [&_error = error, &_block = block] {
                lock_t const lock(_block);
                _error = true;
                throw test_exception("failure");
            });

            sut = interim ^ (executor{make_executor<1>()} & [](auto failedFuture) {
                      check_failure<test_exception>(failedFuture, "failure");
                  });
        }

        wait_until_future_completed(std::move(sut));

        BOOST_REQUIRE(error);
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_GE(1, custom_scheduler<1>::usage_counter());
    }
}

BOOST_AUTO_TEST_CASE(future_recover_failure_during_when_all_on_lvalue) {
    BOOST_TEST_MESSAGE("running future recover while failed when_all on l-value");

    {
        custom_scheduler<0>::reset();
        int result{0};
        auto f1 = async(make_executor<0>(), []() -> int { throw test_exception("failure"); });
        auto f2 = async(make_executor<1>(), [] { return 42; });

        sut = when_all(
                  make_executor<0>(), [](int x, int y) { return x + y; }, f1, f2)
                  .recover([](auto error) {
                      if (error.exception()) {
                          return 815;
                      }
                      return 0;
                  })
                  .then([&](int x) { result = x; });

        wait_until_future_completed(std::move(sut));
        BOOST_REQUIRE_EQUAL(815, result);
    }
    {
        custom_scheduler<0>::reset();
        int result{0};
        auto f1 = async(make_executor<0>(), []() -> int { throw test_exception("failure"); });
        auto f2 = async(make_executor<1>(), [] { return 42; });

        sut = (when_all(
                   make_executor<0>(), [](int x, int y) { return x + y; }, f1, f2) ^
               [](auto error) {
                   if (error.exception()) {
                       return 815;
                   }
                   return 0;
               }) |
              [&](int x) { result = x; };

        wait_until_future_completed(std::move(sut));
        BOOST_REQUIRE_EQUAL(815, result);
    }
}
BOOST_AUTO_TEST_SUITE_END()

// ----------------------------------------------------------------------------
//                             Copyable Values
// ----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_SUITE(future_recover_int, test_fixture<int>)
BOOST_AUTO_TEST_CASE(
    future_recover_int_simple_recover_failure_before_recover_initialized_on_rvalue) {
    BOOST_TEST_MESSAGE("running future int recover, failure before recover initialized on r-value");

    {
        custom_scheduler<0>::reset();
        auto error = false;

        sut = async(make_executor<0>(), [&_error = error]() -> int {
                  _error = true;
                  throw test_exception("failure");
              }).recover([](auto failedFuture) {
            check_failure<test_exception>(failedFuture, "failure");
            return 42;
        });

        auto result = await(std::move(sut));

        BOOST_REQUIRE_EQUAL(42, result);
        BOOST_REQUIRE(error);
    }
    {
        custom_scheduler<0>::reset();
        auto error = false;

        sut = async(make_executor<0>(),
                    [&_error = error]() -> int {
                        _error = true;
                        throw test_exception("failure");
                    }) ^
              [](auto failedFuture) {
                  check_failure<test_exception>(failedFuture, "failure");
                  return 42;
              };

        auto result = await(std::move(sut));

        BOOST_REQUIRE_EQUAL(42, result);
        BOOST_REQUIRE(error);
    }
}

BOOST_AUTO_TEST_CASE(
    future_recover_int_simple_recover_failure_before_recover_initialized_on_lvalue) {
    BOOST_TEST_MESSAGE("running future int recover, failure before recover initialized on l-value");

    {
        custom_scheduler<0>::reset();
        auto error = false;
        auto interim = async(make_executor<0>(), [&_error = error]() -> int {
            _error = true;
            throw test_exception("failure");
        });

        sut = interim.recover([](auto failedFuture) {
            check_failure<test_exception>(failedFuture, "failure");
            return 42;
        });

        auto result = await(std::move(sut));

        BOOST_REQUIRE_EQUAL(42, result);
        BOOST_REQUIRE(error);
    }
    {
        custom_scheduler<0>::reset();
        auto error = false;
        auto interim = async(make_executor<0>(), [&_error = error]() -> int {
            _error = true;
            throw test_exception("failure");
        });

        sut = interim ^ [](auto failedFuture) {
            check_failure<test_exception>(failedFuture, "failure");
            return 42;
        };

        auto result = await(std::move(sut));

        BOOST_REQUIRE_EQUAL(42, result);
        BOOST_REQUIRE(error);
    }
}

BOOST_AUTO_TEST_CASE(
    future_recover_int_simple_recover_failure_after_recover_initialized_on_rvalue) {
    BOOST_TEST_MESSAGE("running future int recover, failure after recover initialized on r-value");

    {
        custom_scheduler<0>::reset();
        auto error = false;
        mutex block;

        {
            lock_t const hold(block);

            sut = async(make_executor<0>(), [&_error = error, &_block = block]() -> int {
                      lock_t const lock(_block);
                      _error = true;
                      throw test_exception("failure");
                  }).recover([](auto failedFuture) {
                check_failure<test_exception>(failedFuture, "failure");
                return 42;
            });
        }

        auto result = await(std::move(sut));

        BOOST_REQUIRE_EQUAL(42, result);
        BOOST_REQUIRE(error);
    }
    {
        custom_scheduler<0>::reset();
        auto error = false;
        mutex block;

        {
            lock_t const hold(block);

            sut = async(make_executor<0>(),
                        [&_error = error, &_block = block]() -> int {
                            lock_t const lock(_block);
                            _error = true;
                            throw test_exception("failure");
                        }) ^
                  [](auto failedFuture) {
                      check_failure<test_exception>(failedFuture, "failure");
                      return 42;
                  };
        }

        auto result = await(std::move(sut));

        BOOST_REQUIRE_EQUAL(42, result);
        BOOST_REQUIRE(error);
    }
}

BOOST_AUTO_TEST_CASE(
    future_recover_int_simple_recover_failure_after_recover_initialized_on_lvalue) {
    BOOST_TEST_MESSAGE("running future int recover, failure after recover initialized on l-value");

    {
        custom_scheduler<0>::reset();
        auto error = false;
        mutex block;

        {
            lock_t const hold(block);

            auto interim = async(make_executor<0>(), [&_error = error, &_block = block]() -> int {
                lock_t const lock(_block);
                _error = true;
                throw test_exception("failure");
            });

            sut = interim.recover([](auto failedFuture) {
                check_failure<test_exception>(failedFuture, "failure");
                return 42;
            });
        }

        auto result = await(std::move(sut));

        BOOST_REQUIRE_EQUAL(42, result);
        BOOST_REQUIRE(error);
    }
    {
        custom_scheduler<0>::reset();
        auto error = false;
        mutex block;

        {
            lock_t const hold(block);

            auto interim = async(make_executor<0>(), [&_error = error, &_block = block]() -> int {
                lock_t const lock(_block);
                _error = true;
                throw test_exception("failure");
            });

            sut = interim ^ [](auto failedFuture) {
                check_failure<test_exception>(failedFuture, "failure");
                return 42;
            };
        }

        auto result = await(std::move(sut));

        BOOST_REQUIRE_EQUAL(42, result);
        BOOST_REQUIRE(error);
    }
}

BOOST_AUTO_TEST_CASE(
    future_recover_int_simple_recover_failure_before_recover_initialized_with_custom_scheduler_on_rvalue) {
    BOOST_TEST_MESSAGE(
        "running future int recover, failure before recover initialized with custom scheduler on r-value");

    {
        auto error = false;

        sut = async(make_executor<0>(), [&_error = error]() -> int {
                  _error = true;
                  throw test_exception("failure");
              }).recover(make_executor<1>(), [](auto failedFuture) {
            check_failure<test_exception>(failedFuture, "failure");
            return 42;
        });

        auto result = await(std::move(sut));

        BOOST_REQUIRE_EQUAL(42, result);
        BOOST_REQUIRE(error);
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_GE(1, custom_scheduler<1>::usage_counter());
    }

    custom_scheduler<0>::reset();
    custom_scheduler<1>::reset();

    {
        auto error = false;

        sut = async(make_executor<0>(),
                    [&_error = error]() -> int {
                        _error = true;
                        throw test_exception("failure");
                    }) ^
              (executor{make_executor<1>()} & [](auto failedFuture) {
                  check_failure<test_exception>(failedFuture, "failure");
                  return 42;
              });

        auto result = await(std::move(sut));

        BOOST_REQUIRE_EQUAL(42, result);
        BOOST_REQUIRE(error);
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_GE(1, custom_scheduler<1>::usage_counter());
    }
}

BOOST_AUTO_TEST_CASE(
    future_recover_int_simple_recover_failure_before_recover_initialized_with_custom_scheduler_on_lvalue) {
    BOOST_TEST_MESSAGE(
        "running future int recover, failure before recover initialized with custom scheduler on l-value");

    {
        auto error = false;
        auto interim = async(make_executor<0>(), [&_error = error]() -> int {
            _error = true;
            throw test_exception("failure");
        });

        wait_until_future_fails<test_exception>(copy(interim));

        sut = interim.recover(make_executor<1>(), [](auto failedFuture) {
            check_failure<test_exception>(failedFuture, "failure");
            return 42;
        });

        auto result = await(std::move(sut));

        BOOST_REQUIRE_EQUAL(42, result);
        BOOST_REQUIRE(error);
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<1>::usage_counter());
    }

    custom_scheduler<0>::reset();
    custom_scheduler<1>::reset();

    {
        auto error = false;
        auto interim = async(make_executor<0>(), [&_error = error]() -> int {
            _error = true;
            throw test_exception("failure");
        });

        wait_until_future_fails<test_exception>(copy(interim));

        sut = interim ^ (executor{make_executor<1>()} & [](auto failedFuture) {
                  check_failure<test_exception>(failedFuture, "failure");
                  return 42;
              });

        auto result = await(std::move(sut));

        BOOST_REQUIRE_EQUAL(42, result);
        BOOST_REQUIRE(error);
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<1>::usage_counter());
    }
}

BOOST_AUTO_TEST_CASE(
    future_recover_int_simple_recover_failure_after_recover_initialized_with_custom_scheduler_on_rvalue) {
    BOOST_TEST_MESSAGE(
        "running future int recover, failure after recover initialized with custom scheduler on r-value");

    {
        auto error = false;
        mutex block;

        {
            lock_t const hold(block);

            sut = async(make_executor<0>(), [&_error = error, &_block = block]() -> int {
                      lock_t const lock(_block);
                      _error = true;
                      throw test_exception("failure");
                  }).recover(make_executor<1>(), [](auto failedFuture) {
                check_failure<test_exception>(failedFuture, "failure");
                return 42;
            });
        }

        auto result = await(std::move(sut));

        BOOST_REQUIRE_EQUAL(42, result);
        BOOST_REQUIRE(error);
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_GE(1, custom_scheduler<1>::usage_counter());
    }

    custom_scheduler<0>::reset();
    custom_scheduler<1>::reset();

    {
        auto error = false;
        mutex block;

        {
            lock_t const hold(block);

            sut = async(make_executor<0>(),
                        [&_error = error, &_block = block]() -> int {
                            lock_t const lock(_block);
                            _error = true;
                            throw test_exception("failure");
                        }) ^
                  (executor{make_executor<1>()} & [](auto failedFuture) {
                      check_failure<test_exception>(failedFuture, "failure");
                      return 42;
                  });
        }

        auto result = await(std::move(sut));

        BOOST_REQUIRE_EQUAL(42, result);
        BOOST_REQUIRE(error);
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_GE(1, custom_scheduler<1>::usage_counter());
    }
}

BOOST_AUTO_TEST_CASE(
    future_recover_int_simple_recover_failure_after_recover_initialized_with_custom_scheduler_on_lvalue) {
    BOOST_TEST_MESSAGE(
        "running future int recover, failure after recover initialized with custom scheduler on l-value");

    {
        auto error = false;
        mutex block;

        {
            lock_t const hold(block);
            auto interim = async(make_executor<0>(), [&_error = error, &_block = block]() -> int {
                lock_t const lock(_block);
                _error = true;
                throw test_exception("failure");
            });

            sut = interim.recover(make_executor<1>(), [](auto failedFuture) {
                check_failure<test_exception>(failedFuture, "failure");
                return 42;
            });
        }

        auto result = await(std::move(sut));

        BOOST_REQUIRE_EQUAL(42, result);
        BOOST_REQUIRE(error);
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_GE(1, custom_scheduler<1>::usage_counter());
    }

    custom_scheduler<0>::reset();
    custom_scheduler<1>::reset();

    {
        auto error = false;
        mutex block;

        {
            lock_t const hold(block);
            auto interim = async(make_executor<0>(), [&_error = error, &_block = block]() -> int {
                lock_t const lock(_block);
                _error = true;
                throw test_exception("failure");
            });

            sut = interim ^ (executor{make_executor<1>()} & [](auto failedFuture) {
                      check_failure<test_exception>(failedFuture, "failure");
                      return 42;
                  });
        }

        auto result = await(std::move(sut));

        BOOST_REQUIRE_EQUAL(42, result);
        BOOST_REQUIRE(error);
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_GE(1, custom_scheduler<1>::usage_counter());
    }
}

BOOST_AUTO_TEST_CASE(future_recover_int_with_broken_promise) {
    BOOST_TEST_MESSAGE("running future int recover with broken promise");

    {
        auto check{false};
        sut = [&check]() {
            auto p{package<int(int)>(immediate_executor, [](int x) { return x; })};
            return p.second.recover([&check](const auto& f) {
                check = true;
                try {
                    return *f.get_try();
                } catch (const exception&) {
                    throw;
                }
            });
        }();

        check_failure<future_error>(sut, "broken promise");
        BOOST_REQUIRE(check);
    }
    {
        auto check{false};
        sut = [&check]() {
            auto p{package<int(int)>(immediate_executor, [](int x) { return x; })};
            return p.second ^ [&check](const auto& f) {
                check = true;
                try {
                    return *f.get_try();
                } catch (const exception&) {
                    throw;
                }
            };
        }();

        check_failure<future_error>(sut, "broken promise");
        BOOST_REQUIRE(check);
    }
}

BOOST_AUTO_TEST_SUITE_END()

// ----------------------------------------------------------------------------
//                             Move-only values
// ----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_SUITE(future_recover_move_only_type, test_fixture<move_only>)
BOOST_AUTO_TEST_CASE(
    future_recover_move_only_type_recover_failure_before_recover_initialized_on_rvalue) {
    BOOST_TEST_MESSAGE(
        "running future move only type recover, failure before recover initialized on r-value");

    {
        custom_scheduler<0>::reset();
        auto error = false;

        sut = async(make_executor<0>(), [&_error = error]() -> move_only {
                  _error = true;
                  throw test_exception("failure");
              }).recover([](auto failedFuture) {
            check_failure<test_exception>(failedFuture, "failure");
            return move_only(42);
        });

        auto result = await(std::move(sut));

        BOOST_REQUIRE_EQUAL(42, result.member());
        BOOST_REQUIRE(error);
    }
    {
        custom_scheduler<0>::reset();
        auto error = false;

        sut = async(make_executor<0>(),
                    [&_error = error]() -> move_only {
                        _error = true;
                        throw test_exception("failure");
                    }) ^
              [](auto failedFuture) {
                  check_failure<test_exception>(failedFuture, "failure");
                  return move_only(42);
              };

        auto result = await(std::move(sut));

        BOOST_REQUIRE_EQUAL(42, result.member());
        BOOST_REQUIRE(error);
    }
}

BOOST_AUTO_TEST_CASE(future_recover_move_only_types_recover_failure_after_recover_initialized) {
    BOOST_TEST_MESSAGE("running future move only type recover, failure after recover initialized");

    {
        custom_scheduler<0>::reset();
        auto error = false;
        mutex block;
        {
            lock_t const hold(block);

            sut = async(make_executor<0>(), [&_error = error, &_block = block]() -> move_only {
                      lock_t const lock(_block);
                      _error = true;
                      throw test_exception("failure");
                  }).recover([](auto failedFuture) {
                check_failure<test_exception>(failedFuture, "failure");
                return move_only(42);
            });
        }

        auto result = await(std::move(sut));

        BOOST_REQUIRE_EQUAL(42, result.member());
        BOOST_REQUIRE(error);
    }
    {
        custom_scheduler<0>::reset();
        auto error = false;
        mutex block;
        {
            lock_t const hold(block);

            sut = async(make_executor<0>(),
                        [&_error = error, &_block = block]() -> move_only {
                            lock_t const lock(_block);
                            _error = true;
                            throw test_exception("failure");
                        }) ^
                  [](auto failedFuture) {
                      check_failure<test_exception>(failedFuture, "failure");
                      return move_only(42);
                  };
        }

        auto result = await(std::move(sut));

        BOOST_REQUIRE_EQUAL(42, result.member());
        BOOST_REQUIRE(error);
    }
}

BOOST_AUTO_TEST_CASE(
    future_recover_move_only_type_recover_failure_before_recover_initialized_with_custom_scheduler_on_rvalue) {
    BOOST_TEST_MESSAGE(
        "running future move only type recover, failure before recover initialized with custom scheduler on r-value");

    {
        auto error = false;

        sut = async(make_executor<0>(), [&_error = error]() -> move_only {
                  _error = true;
                  throw test_exception("failure");
              }).recover(make_executor<1>(), [](auto failedFuture) {
            check_failure<test_exception>(failedFuture, "failure");
            return move_only(42);
        });

        auto result = await(std::move(sut));

        BOOST_REQUIRE_EQUAL(42, result.member());
        BOOST_REQUIRE(error);
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_GE(1, custom_scheduler<1>::usage_counter());
    }

    custom_scheduler<0>::reset();
    custom_scheduler<1>::reset();

    {
        auto error = false;

        sut = async(make_executor<0>(),
                    [&_error = error]() -> move_only {
                        _error = true;
                        throw test_exception("failure");
                    }) ^
              (executor{make_executor<1>()} & [](auto failedFuture) {
                  check_failure<test_exception>(failedFuture, "failure");
                  return move_only(42);
              });

        auto result = await(std::move(sut));

        BOOST_REQUIRE_EQUAL(42, result.member());
        BOOST_REQUIRE(error);
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_GE(1, custom_scheduler<1>::usage_counter());
    }
}

BOOST_AUTO_TEST_CASE(
    future_recover_move_only_types_recover_failure_after_recover_initialized_with_custom_scheduler_on_rvalue) {
    BOOST_TEST_MESSAGE(
        "running future move only type recover, failure after recover initialized with custom scheduler on r-value");

    {
        auto error = false;
        mutex block;
        {
            lock_t const hold(block);
            sut = async(make_executor<0>(), [&_error = error, &_block = block]() -> move_only {
                      lock_t const lock(_block);
                      _error = true;
                      throw test_exception("failure");
                  }).recover(make_executor<1>(), [](auto failedFuture) {
                check_failure<test_exception>(failedFuture, "failure");
                return move_only(42);
            });
        }

        auto result = await(std::move(sut));

        BOOST_REQUIRE_EQUAL(42, result.member());
        BOOST_REQUIRE(error);
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_GE(1, custom_scheduler<1>::usage_counter());
    }

    custom_scheduler<0>::reset();
    custom_scheduler<1>::reset();

    {
        auto error = false;
        mutex block;
        {
            lock_t const hold(block);
            sut = async(make_executor<0>(),
                        [&_error = error, &_block = block]() -> move_only {
                            lock_t const lock(_block);
                            _error = true;
                            throw test_exception("failure");
                        }) ^
                  (executor{make_executor<1>()} & [](auto failedFuture) {
                      check_failure<test_exception>(failedFuture, "failure");
                      return move_only(42);
                  });
        }

        auto result = await(std::move(sut));

        BOOST_REQUIRE_EQUAL(42, result.member());
        BOOST_REQUIRE(error);
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_GE(1, custom_scheduler<1>::usage_counter());
    }
}

BOOST_AUTO_TEST_CASE(future_recover_move_only_with_broken_promise) {
    BOOST_TEST_MESSAGE("running future move-only recover with broken promise");

    {
        auto check{false};
        sut = [&check]() {
            auto p{
                package<move_only(move_only)>(immediate_executor, [](move_only x) { return x; })};
            return std::move(p.second).recover([&check](auto f) {
                check = true;
                try {
                    return *std::move(f.get_try());
                } catch (const exception&) {
                    throw;
                }
            });
        }();

        check_failure<future_error>(sut, "broken promise");
        BOOST_REQUIRE(check);
    }
    {
        auto check{false};
        sut = [&check]() {
            auto p{
                package<move_only(move_only)>(immediate_executor, [](move_only x) { return x; })};
            return std::move(p.second) ^ [&check](auto f) {
                check = true;
                try {
                    return *std::move(f.get_try());
                } catch (const exception&) {
                    throw;
                }
            };
        }();

        check_failure<future_error>(sut, "broken promise");
        BOOST_REQUIRE(check);
    }
}

BOOST_AUTO_TEST_SUITE_END()
