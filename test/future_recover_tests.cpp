/*-
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

namespace
{
    template <typename T>
    class scoped_bool_setter
    {
        T& _v;
    public:
        explicit scoped_bool_setter(T& v) : _v(v) {}

        ~scoped_bool_setter() { _v = true; }
    };
}

BOOST_FIXTURE_TEST_SUITE(future_recover_void, test_fixture<void>)
    BOOST_AUTO_TEST_CASE(future_recover_recover_failure_before_recover_initialized_on_rvalue) {
        BOOST_TEST_MESSAGE("running future recover, failure before recover initialized on r-value");

        std::atomic_bool check{ false };
        auto error = false;

        sut = async(custom_scheduler<0>(), [&_error = error] {
            _error = true;
            throw test_exception("failure"); })
            .recover([&_check = check](auto failedFuture) {
                check_failure<test_exception>(failedFuture, "failure");
                _check = true;
            });

        wait_until_future_completed(sut);
        while (!check.load()) {}

        BOOST_REQUIRE(check);
        BOOST_REQUIRE(error);
        BOOST_REQUIRE_EQUAL(2, custom_scheduler<0>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_recover_recover_failure_before_recover_initialized_on_lvalue) {
        BOOST_TEST_MESSAGE("running future recover, failure before recover initialized on l-value");

        std::atomic_bool check{ false };
        auto error = false;

        auto interim = async(custom_scheduler<0>(), [&_error = error] {
            _error = true;
            throw test_exception("failure"); });

        sut = interim.recover([&_check = check](auto failedFuture) {
                check_failure<test_exception>(failedFuture, "failure");
                _check = true;
            });

        wait_until_future_completed(sut);
        while (!check.load()) {}

        BOOST_REQUIRE(check);
        BOOST_REQUIRE(error);
        BOOST_REQUIRE_EQUAL(2, custom_scheduler<0>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_recover_recover_failure_before_recover_initialized_with_custom_scheduler_on_rvalue) {
        BOOST_TEST_MESSAGE("running future recover, failure before recover initialized, with custom scheduler on r-value");

        std::atomic_bool check{ false };
        auto error = false;

        sut = async(custom_scheduler<0>(), [&_error = error] {
                                            _error = true;
                                            throw test_exception("failure");})
            .recover(custom_scheduler<1>(), [&_check = check](auto failedFuture) {
                                            check_failure<test_exception>(failedFuture, "failure");
                                            _check = true;});

        wait_until_future_completed(sut);
        while (!check.load()) {}

        BOOST_REQUIRE(check);
        BOOST_REQUIRE(error);
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<1>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_recover_recover_failure_before_recover_initialized_with_custom_scheduler_on_lvalue) {
        BOOST_TEST_MESSAGE("running future recover, failure before recover initialized, with custom scheduler on l-value");

        std::atomic_bool check{ false };
        auto error = false;

        auto interim = async(custom_scheduler<0>(), [&_error = error] {
            _error = true;
            throw test_exception("failure"); });
        sut = interim.recover(custom_scheduler<1>(), [&_check = check](auto failedFuture) {
                check_failure<test_exception>(failedFuture, "failure");
                _check = true; });

        wait_until_future_completed(sut);
        while (!check.load()) {}

        BOOST_REQUIRE(check);
        BOOST_REQUIRE(error);
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<1>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_recover_recover_failure_after_recover_initialized_on_rvalue) {
        BOOST_TEST_MESSAGE("running future recover, failure after recover initialized on r-value");

        std::atomic_bool check{ false };
        auto error = false;
        std::mutex block;

        {
            std::unique_lock<std::mutex> hold(block);
            sut = async(custom_scheduler<0>(), [&_error = error, &_block = block] {
                std::unique_lock<std::mutex> lock(_block);
                _error = true;
                throw test_exception("failure");
            }).recover([&_check = check](auto failedFuture) {
                check_failure<test_exception>(failedFuture, "failure");
                _check = true;
            });
        }
        wait_until_future_completed(sut);
        while (!check.load()) {}

        BOOST_REQUIRE(check);
        BOOST_REQUIRE(error);
    }

    BOOST_AUTO_TEST_CASE(future_recover_recover_failure_after_recover_initialized_on_lvalue) {
        BOOST_TEST_MESSAGE("running future recover, failure after recover initialized on l-value");

        std::atomic_bool check{ false };
        auto error = false;
        std::mutex block;

        {
            std::unique_lock<std::mutex> hold(block);
            auto interim = async(custom_scheduler<0>(), [&_error = error, &_block = block] {
                std::unique_lock<std::mutex> lock(_block);
                _error = true;
                throw test_exception("failure");
            });
            sut = interim.recover([&_check = check](auto failedFuture) {
                check_failure<test_exception>(failedFuture, "failure");
                _check = true;
            });
        }
        wait_until_future_completed(sut);
        while (!check.load()) {}

        BOOST_REQUIRE(check);
        BOOST_REQUIRE(error);
    }

    BOOST_AUTO_TEST_CASE(future_recover_recover_failure_after_recover_initialized_with_custom_scheduler_on_rvalue) {
        BOOST_TEST_MESSAGE("running future recover, failure after recover initialized with custom scheduler on r-value");
        std::atomic_bool check{ false };
        auto error = false;
        std::mutex block;

        {
            std::unique_lock<std::mutex> hold(block);
            sut = async(custom_scheduler<0>(), [&_error = error, &_block = block] {
                std::unique_lock<std::mutex> lock(_block);
                _error = true;
                throw test_exception("failure");
            }).recover(custom_scheduler<1>(), [&_check = check](auto failedFuture) {
                check_failure<test_exception>(failedFuture, "failure");
                _check = true;
            });
        }
        wait_until_future_completed(sut);
        while (!check.load()) {}

        BOOST_REQUIRE(check);
        BOOST_REQUIRE(error);
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<1>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_recover_recover_failure_after_recover_initialized_with_custom_scheduler_on_lvalue) {
        BOOST_TEST_MESSAGE("running future recover, failure after recover initialized with custom scheduler on l-value");
        std::atomic_bool check{ false };
        auto error = false;
        std::mutex block;

        {
            std::unique_lock<std::mutex> hold(block);
            auto interim = async(custom_scheduler<0>(), [&_error = error, &_block = block] {
                std::unique_lock<std::mutex> lock(_block);
                _error = true;
                throw test_exception("failure");
            });
            sut = interim.recover(custom_scheduler<1>(), [&_check = check](auto failedFuture) {
                check_failure<test_exception>(failedFuture, "failure");
                _check = true;
            });
        }
        wait_until_future_completed(sut);
        while (!check.load()) {}

        BOOST_REQUIRE(check);
        BOOST_REQUIRE(error);
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<1>::usage_counter());
    }
BOOST_AUTO_TEST_SUITE_END()



BOOST_FIXTURE_TEST_SUITE(future_recover_int, test_fixture<int>)
    BOOST_AUTO_TEST_CASE(future_recover_int_simple_recover_failure_before_recover_initialized_on_rvalue) {
        BOOST_TEST_MESSAGE("running future int recover, failure before recover initialized on r-value");
        std::atomic_bool check{ false };
        auto error = false;
        
        sut = async(custom_scheduler<0>(), [&_error = error]()->int {
            _error = true;
            throw test_exception("failure");
        }).recover([&_check = check](auto&& failedFuture) {
            check_failure<test_exception>(failedFuture, "failure");
            _check = true;
            return 42;
        });

        wait_until_future_completed(sut);
        while(!check.load()) {}

        BOOST_REQUIRE_EQUAL(42, sut.get_try().value());
        BOOST_REQUIRE(check);
        BOOST_REQUIRE(error);
    }

    BOOST_AUTO_TEST_CASE(future_recover_int_simple_recover_failure_before_recover_initialized_on_lvalue) {
        BOOST_TEST_MESSAGE("running future int recover, failure before recover initialized on l-value");
        std::atomic_bool check{ false };
        auto error = false;

        auto interim = async(custom_scheduler<0>(), [&_error = error]()->int {
            _error = true;
            throw test_exception("failure");
        });
        sut = interim.recover([&_check = check](auto failedFuture) {
            check_failure<test_exception>(failedFuture, "failure");
            _check = true;
            return 42;
        });

        wait_until_future_completed(sut);
        while (!check.load()) {}

        BOOST_REQUIRE_EQUAL(42, sut.get_try().value());
        BOOST_REQUIRE(check);
        BOOST_REQUIRE(error);
    }

    BOOST_AUTO_TEST_CASE(future_recover_int_simple_recover_failure_after_recover_initialized_on_rvalue) {
        BOOST_TEST_MESSAGE("running future int recover, failure after recover initialized on r-value");
        std::atomic_bool check{ false };
        auto error = false;
        std::mutex block;

        {
            std::unique_lock<std::mutex> hold(block);
            sut = async(custom_scheduler<0>(), [&_error = error, &_block = block]()->int {
                std::unique_lock<std::mutex> lock(_block);
                _error = true;
                throw test_exception("failure");
            }).recover([&_check = check](auto failedFuture) {
                check_failure<test_exception>(failedFuture, "failure");
                _check = true;
                return 42;
            });
        }

        wait_until_future_completed(sut);
        while (!check.load()) {}

        BOOST_REQUIRE_EQUAL(42, sut.get_try().value());
        BOOST_REQUIRE(check);
        BOOST_REQUIRE(error);
    }

    BOOST_AUTO_TEST_CASE(future_recover_int_simple_recover_failure_after_recover_initialized_on_lvalue) {
        BOOST_TEST_MESSAGE("running future int recover, failure after recover initialized on l-value");
        std::atomic_bool check{ false };
        auto error = false;
        std::mutex block;

        {
            std::unique_lock<std::mutex> hold(block);
            auto interim = async(custom_scheduler<0>(), [&_error = error, &_block = block]()->int {
                std::unique_lock<std::mutex> lock(_block);
                _error = true;
                throw test_exception("failure");
            });
            sut = interim.recover([&_check = check](auto failedFuture) {
                check_failure<test_exception>(failedFuture, "failure");
                _check = true;
                return 42;
            });
        }

        wait_until_future_completed(sut);
        while (!check.load()) {}

        BOOST_REQUIRE_EQUAL(42, sut.get_try().value());
        BOOST_REQUIRE(check);
        BOOST_REQUIRE(error);
    }


    BOOST_AUTO_TEST_CASE(future_recover_int_simple_recover_failure_before_recover_initialized_with_custom_scheduler_on_rvalue) {
        BOOST_TEST_MESSAGE("running future int recover, failure before recover initialized with custom scheduler on r-value");
        std::atomic_bool check{ false };
        auto error = false;

        sut = async(custom_scheduler<0>(), [&_error = error]()->int {
            _error = true;
            throw test_exception("failure");
        }).recover(custom_scheduler<1>(), [&_check = check](auto failedFuture) {
            check_failure<test_exception>(failedFuture, "failure");
            _check = true;
            return 42;
        });

        wait_until_future_completed(sut);
        while (!check.load()) {}

        BOOST_REQUIRE_EQUAL(42, sut.get_try().value());
        BOOST_REQUIRE(check);
        BOOST_REQUIRE(error);
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<1>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_recover_int_simple_recover_failure_before_recover_initialized_with_custom_scheduler_on_lvalue) {
        BOOST_TEST_MESSAGE("running future int recover, failure before recover initialized with custom scheduler on l-value");
        std::atomic_bool check{ false };
        auto error = false;

        auto interim = async(custom_scheduler<0>(), [&_error = error]()->int {
            _error = true;
            throw test_exception("failure");
        });
        sut = interim.recover(custom_scheduler<1>(), [&_check = check](auto failedFuture) {
            check_failure<test_exception>(failedFuture, "failure");
            _check = true;
            return 42;
        });

        wait_until_future_completed(sut);
        while (!check.load()) {}

        BOOST_REQUIRE_EQUAL(42, sut.get_try().value());
        BOOST_REQUIRE(check);
        BOOST_REQUIRE(error);
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<1>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_recover_int_simple_recover_failure_after_recover_initialized_with_custom_scheduler_on_rvalue) {
        BOOST_TEST_MESSAGE("running future int recover, failure after recover initialized with custom scheduler on r-value");
        std::atomic_bool check{ false };
        auto error = false;
        std::mutex block;

        {
            std::unique_lock<std::mutex> hold(block);
            sut = async(custom_scheduler<0>(), [&_error = error, &_block = block]()->int {
                std::unique_lock<std::mutex> lock(_block);
                _error = true;
                throw test_exception("failure");
            }).recover(custom_scheduler<1>(), [&_check = check](auto failedFuture) {
                check_failure<test_exception>(failedFuture, "failure");
                _check = true;
                return 42;
            });
        }

        wait_until_future_completed(sut);
        while (!check.load()) {}

        BOOST_REQUIRE_EQUAL(42, sut.get_try().value());
        BOOST_REQUIRE(check);
        BOOST_REQUIRE(error);
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<1>::usage_counter());
    }

    BOOST_AUTO_TEST_CASE(future_recover_int_simple_recover_failure_after_recover_initialized_with_custom_scheduler_on_lvalue) {
        BOOST_TEST_MESSAGE("running future int recover, failure after recover initialized with custom scheduler on l-value");
        std::atomic_bool check{ false };
        auto error = false;
        std::mutex block;

        {
            std::unique_lock<std::mutex> hold(block);
            auto interim = async(custom_scheduler<0>(), [&_error = error, &_block = block]()->int {
                std::unique_lock<std::mutex> lock(_block);
                _error = true;
                throw test_exception("failure");
            });
            sut = interim.recover(custom_scheduler<1>(), [&_check = check](auto failedFuture) {
                check_failure<test_exception>(failedFuture, "failure");
                _check = true;
                return 42;
            });
        }

        wait_until_future_completed(sut);
        while (!check.load()) {}

        BOOST_REQUIRE_EQUAL(42, sut.get_try().value());
        BOOST_REQUIRE(check);
        BOOST_REQUIRE(error);
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<1>::usage_counter());
    }

BOOST_AUTO_TEST_SUITE_END()



BOOST_FIXTURE_TEST_SUITE(future_recover_move_only_type, test_fixture<std::unique_ptr<int>>)
    BOOST_AUTO_TEST_CASE(future_recover_move_only_type_recover_failure_before_recover_initialized_on_rvalue) {
        BOOST_TEST_MESSAGE("running future move only type recover, failure before recover initialized on r-value");
        
        auto error = false;

        sut = async(custom_scheduler<0>(), [&_error = error]()->std::unique_ptr<int> {
            _error = true;
            throw test_exception("failure");
        }).recover([](auto failedFuture) {
            check_failure<test_exception>(failedFuture, "failure");
            auto result = std::make_unique<int>();
            *result = 42;
            return std::move(result);
        });

        auto result = wait_until_future_r_completed(sut);

        BOOST_REQUIRE_EQUAL(42, **result);
        BOOST_REQUIRE(error);
    }


    BOOST_AUTO_TEST_CASE(future_recover_move_only_types_recover_failure_after_recover_initialized) {
        BOOST_TEST_MESSAGE("running future move only type recover, failure after recover initialized");

        auto error = false;
        std::mutex block;
        {
            std::unique_lock<std::mutex> hold(block);
            sut = async(custom_scheduler<0>(), [&_error = error, &_block = block]()->std::unique_ptr<int> {
                std::unique_lock<std::mutex> lock(_block);
                _error = true;
                throw test_exception("failure");
            }).recover([](auto failedFuture) {
                check_failure<test_exception>(failedFuture, "failure");
                auto result = std::make_unique<int>();
                *result = 42;
                return std::move(result);
            });
        }

        auto result = wait_until_future_r_completed(sut);

        BOOST_REQUIRE_EQUAL(42, **result);
        BOOST_REQUIRE(error);
    }


    BOOST_AUTO_TEST_CASE(future_recover_move_only_type_recover_failure_before_recover_initialized_with_custom_scheduler_on_rvalue) {
        BOOST_TEST_MESSAGE("running future move only type recover, failure before recover initialized with custom scheduler on r-value");

        auto error = false;

        sut = async(custom_scheduler<0>(), [&_error = error]()->std::unique_ptr<int> {
            _error = true;
            throw test_exception("failure");
        }).recover(custom_scheduler<1>(), [](auto failedFuture) {
            check_failure<test_exception>(failedFuture, "failure");
            auto result = std::make_unique<int>();
            *result = 42;
            return std::move(result);
        });

        auto result = wait_until_future_r_completed(sut);

        BOOST_REQUIRE_EQUAL(42, **result);
        BOOST_REQUIRE(error);
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<1>::usage_counter());
    }


    BOOST_AUTO_TEST_CASE(future_recover_move_only_types_recover_failure_after_recover_initialized_with_custom_scheduler_on_rvalue) {
        BOOST_TEST_MESSAGE("running future move only type recover, failure after recover initialized with custom scheduler on r-value");

        auto error = false;

        std::mutex block;
        {
            std::unique_lock<std::mutex> hold(block);
            sut = async(custom_scheduler<0>(), [&_error = error, &_block = block]()->std::unique_ptr<int> {
                std::unique_lock<std::mutex> lock(_block);
                _error = true;
                throw test_exception("failure");
            }).recover(custom_scheduler<1>(), [](auto failedFuture) {
                check_failure<test_exception>(failedFuture, "failure");
                auto result = std::make_unique<int>();
                *result = 42;
                return std::move(result);
            });
        }

        auto result = wait_until_future_r_completed(sut);

        BOOST_REQUIRE_EQUAL(42, **result);
        BOOST_REQUIRE(error);
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
        BOOST_REQUIRE_EQUAL(1, custom_scheduler<1>::usage_counter());
    }

BOOST_AUTO_TEST_SUITE_END()
