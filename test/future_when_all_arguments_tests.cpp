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

#include <string>

#include "future_test_helper.hpp"
#include <stlab/test/model.hpp>

using namespace stlab;
using namespace future_test_helper;

BOOST_FIXTURE_TEST_SUITE(future_when_all_args_int, test_fixture<int>)
BOOST_AUTO_TEST_CASE(future_when_all_args_int_with_one_element) {
    BOOST_TEST_MESSAGE("running future when_all int with one element");

    auto f1 = async(make_executor<0>(), [] { return 42; });
    sut = when_all(make_executor<1>(), [](auto x) { return x + x; }, f1);

    check_valid_future(sut);
    wait_until_future_completed(sut);

    BOOST_REQUIRE_EQUAL(42 + 42, *sut.get_try());
    BOOST_REQUIRE_LE(1, custom_scheduler<0>::usage_counter());
    BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_CASE(future_when_all_args_int_with_many_elements) {
    BOOST_TEST_MESSAGE("running future when_all args int with many elements");

    auto f1 = async(make_executor<0>(), [] { return 1; });
    auto f2 = async(make_executor<0>(), [] { return 2; });
    auto f3 = async(make_executor<0>(), [] { return 3; });
    auto f4 = async(make_executor<0>(), [] { return 5; });

    sut = when_all(
        make_executor<1>(),
        [](int x1, int x2, int x3, int x4) { return 7 * x1 + 11 * x2 + 13 * x3 + 17 * x4; }, f1, f2,
        f3, f4);

    check_valid_future(sut);
    wait_until_future_completed(sut);

    BOOST_REQUIRE_EQUAL(1 * 7 + 2 * 11 + 3 * 13 + 5 * 17, *sut.get_try());
    BOOST_REQUIRE_LE(4, custom_scheduler<0>::usage_counter());
    BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_CASE(future_when_all_args_int_with_ready_element) {
    BOOST_TEST_MESSAGE("running future when_all int with ready element");

    sut = when_all(make_executor<1>(), [](auto x) { return x + x; },
        make_ready_future<int>(42, immediate_executor));

    check_valid_future(sut);
    wait_until_future_completed(sut);

    BOOST_REQUIRE_EQUAL(42 + 42, *sut.get_try());
    BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_CASE(future_when_all_args_int_with_executor) {
    BOOST_TEST_MESSAGE("running future when_all int with ready element");

    sut = stlab::when_all(make_executor<1>(), [](auto x, auto y) { return x + y; },
                             stlab::make_ready_future<int>(42, stlab::immediate_executor),
                             stlab::make_ready_future<int>(42, stlab::immediate_executor));

    check_valid_future(sut);
    wait_until_future_completed(sut);

    BOOST_REQUIRE_EQUAL(42 + 42, *sut.get_try());
    BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_CASE(future_when_all_args_int_with_two_ready_element) {
    BOOST_TEST_MESSAGE("running future when_all int with two ready element");

    sut = when_all(make_executor<1>(), [](auto x, auto y) { return x + y; },
                   make_ready_future<int>(42, immediate_executor),
                   make_ready_future<int>(42, immediate_executor));

    check_valid_future(sut);
    wait_until_future_completed(sut);

    BOOST_REQUIRE_EQUAL(42 + 42, *sut.get_try());
    BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_CASE(future_when_all_args) {

    auto main_thread_id = std::this_thread::get_id();
    auto sut = when_all(make_executor<1>(), [] { 
        return std::this_thread::get_id(); 
    }, make_ready_future(stlab::immediate_executor));

    wait_until_future_completed(sut);

    BOOST_REQUIRE(main_thread_id != *sut.get_try());
    BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_CASE(future_when_all_Arguments_with_mutable_task) {
    BOOST_TEST_MESSAGE("future when all arguments with mutable task");

    struct mutable_int
    {
        int i = 0;
        auto operator()() {
            ++i;
            return i;
        }
    };
    mutable_int func1, func2;

    auto sut = when_all(stlab::default_executor, [](auto f1, auto f2){ return f1() + f2(); },
        async(stlab::default_executor, [func = std::move(func1)]() mutable {
            func();
            return std::move(func);
        }),
        async(stlab::default_executor, [func = std::move(func2)]() mutable {
            func();
            return std::move(func);
        })
        );

    BOOST_REQUIRE_EQUAL(4, stlab::blocking_get(sut));
}
BOOST_AUTO_TEST_CASE(future_when_all_arguments_with_mutable_move_onlytask) {
    BOOST_TEST_MESSAGE("future when all arguments with mutable move only task");

    struct mutable_move_only
    {
        move_only i;
        auto operator()() {
            i = move_only{i.member() + 1};
            return std::move(i);
        }
    };
    mutable_move_only func1, func2;

    auto sut = when_all(stlab::default_executor, [](auto f1, auto f2){ return f1().member() + f2().member(); },
                        async(stlab::default_executor, [func = std::move(func1)]() mutable {
                            func();
                            return std::move(func);
                        }),
                        async(stlab::default_executor, [func = std::move(func2)]() mutable {
                            func();
                            return std::move(func);
                        })
    );

    BOOST_REQUIRE_EQUAL(4, stlab::blocking_get(sut));
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(future_when_all_args_move_only, test_fixture<move_only>)
BOOST_AUTO_TEST_CASE(future_when_all_args_move_only_with_one_element) {
  BOOST_TEST_MESSAGE("running future when_all move_only with one element");

  auto f1 = async(make_executor<0>(), [] { return move_only(42); });
  sut = when_all(make_executor<1>(), [](auto x) { return move_only(x.member() + x.member()); }, std::move(f1));

  check_valid_future(sut);
  wait_until_future_completed(sut);

  BOOST_REQUIRE_EQUAL(42 + 42, (*sut.get_try()).member());
  BOOST_REQUIRE_LE(1, custom_scheduler<0>::usage_counter());
  BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_CASE(future_when_all_args_move_only_with_many_elements) {
  BOOST_TEST_MESSAGE("running future when_all args move_only with many elements");

  auto f1 = async(make_executor<0>(), [] { return move_only(1); });
  auto f2 = async(make_executor<0>(), [] { return move_only(2); });
  auto f3 = async(make_executor<0>(), [] { return move_only(3); });
  auto f4 = async(make_executor<0>(), [] { return move_only(5); });

  sut = when_all(
    make_executor<1>(),
    [](auto x1, auto x2, auto x3, auto x4) { return move_only(7 * x1.member() + 11 * x2.member() + 13 * x3.member() + 17 * x4.member()); }, 
    std::move(f1), std::move(f2), std::move(f3), std::move(f4));

  check_valid_future(sut);
  wait_until_future_completed(sut);

  BOOST_REQUIRE_EQUAL(1 * 7 + 2 * 11 + 3 * 13 + 5 * 17, (*sut.get_try()).member());
  BOOST_REQUIRE_LE(4, custom_scheduler<0>::usage_counter());
  BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_FIXTURE_TEST_SUITE(future_when_all_args_string, test_fixture<std::string>)
BOOST_AUTO_TEST_CASE(future_when_all_args_with_different_types) {
    BOOST_TEST_MESSAGE("running future when_all args with different types");

    auto f1 = async(make_executor<0>(), [] { return 1; });
    auto f2 = async(make_executor<0>(), [] { return 3.1415; });
    auto f3 = async(make_executor<0>(), [] { return std::string("Don't panic!"); });
    auto f4 = async(make_executor<0>(), [] { return std::vector<size_t>(2, 3); });

    sut = when_all(make_executor<1>(),
                   [](int x1, double x2, const std::string& x3, const std::vector<size_t>& x4) {
                       std::stringstream st;
                       st << x1 << " " << x2 << " " << x3 << " " << x4[0] << " " << x4[1];
                       return st.str();
                   },
                   f1, f2, f3, f4);

    check_valid_future(sut);
    wait_until_future_completed(sut);

    BOOST_REQUIRE_EQUAL(std::string("1 3.1415 Don't panic! 3 3"), *sut.get_try());
    BOOST_REQUIRE_LE(4, custom_scheduler<0>::usage_counter());
    BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
}
BOOST_AUTO_TEST_SUITE_END()

// ----------------------------------------------------------------------------
//                             Error cases
// ----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_SUITE(future_when_all_args_int_failure, test_fixture<int>)
BOOST_AUTO_TEST_CASE(future_when_all_args_int_failure_with_one_element) {
    BOOST_TEST_MESSAGE("running future when_all int with range of one element");

    auto f1 = async(make_executor<0>(), []() -> int { throw test_exception("failure"); });
    sut = when_all(make_executor<1>(), [](auto x) { return x + x; }, f1);

    wait_until_future_fails<test_exception>(sut);

    check_failure<test_exception>(sut, "failure");
    BOOST_REQUIRE_LE(1, custom_scheduler<0>::usage_counter());
    BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_CASE(future_when_all_args_int_with_many_elements_one_failing) {
    BOOST_TEST_MESSAGE("running future when_all args int with many elements one failing");

    auto f1 = async(make_executor<0>(), [] { return 1; });
    auto f2 = async(make_executor<0>(), []() -> int { throw test_exception("failure"); });
    auto f3 = async(make_executor<0>(), [] { return 3; });
    auto f4 = async(make_executor<0>(), [] { return 5; });

    sut = when_all(
        make_executor<1>(),
        [](int x1, int x2, int x3, int x4) { return 7 * x1 + 11 * x2 + 13 * x3 + 17 * x4; }, f1, f2,
        f3, f4);

    wait_until_future_fails<test_exception>(sut);

    check_failure<test_exception>(sut, "failure");
    BOOST_REQUIRE_LE(4, custom_scheduler<0>::usage_counter());
    BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_CASE(future_when_all_args_int_with_many_elements_all_failing) {
    BOOST_TEST_MESSAGE("running future when_all args int with many elements all failing");

    auto f1 = async(make_executor<0>(), []() -> int { throw test_exception("failure"); });
    auto f2 = async(make_executor<0>(), []() -> int { throw test_exception("failure"); });
    auto f3 = async(make_executor<0>(), []() -> int { throw test_exception("failure"); });
    auto f4 = async(make_executor<0>(), []() -> int { throw test_exception("failure"); });

    sut = when_all(
        make_executor<1>(),
        [](int x1, int x2, int x3, int x4) { return 7 * x1 + 11 * x2 + 13 * x3 + 17 * x4; }, f1, f2,
        f3, f4);

    wait_until_future_fails<test_exception>(sut);

    check_failure<test_exception>(sut, "failure");
    BOOST_REQUIRE_LE(4, custom_scheduler<0>::usage_counter());
    BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(future_when_all_args_string_failure, test_fixture<std::string>)
BOOST_AUTO_TEST_CASE(future_when_all_args_with_different_types_one_failing) {
    BOOST_TEST_MESSAGE("running future when_all args with different types one failing");

    auto f1 = async(make_executor<0>(), [] { return 1; });
    auto f2 = async(make_executor<0>(), [] { return 3.1415; });
    auto f3 =
        async(make_executor<0>(), []() -> std::string { throw test_exception("failure"); });
    auto f4 = async(make_executor<0>(), [] { return std::vector<size_t>(2, 3); });

    sut = when_all(make_executor<1>(),
                   [](int x1, double x2, const std::string& x3, const std::vector<size_t>& x4) {
                       std::stringstream st;
                       st << x1 << " " << x2 << " " << x3 << " " << x4[0] << " " << x4[1];
                       return st.str();
                   },
                   f1, f2, f3, f4);

    wait_until_future_fails<test_exception>(sut);

    check_failure<test_exception>(sut, "failure");
    BOOST_REQUIRE_LE(4, custom_scheduler<0>::usage_counter());
    BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_CASE(future_when_all_args_with_different_types_all_failing) {
    BOOST_TEST_MESSAGE("running future when_all args with different types all failing");

    auto f1 = async(make_executor<0>(), []() -> int { throw test_exception("failure"); });
    auto f2 = async(make_executor<0>(), []() -> double { throw test_exception("failure"); });
    auto f3 =
        async(make_executor<0>(), []() -> std::string { throw test_exception("failure"); });
    auto f4 = async(make_executor<0>(),
                    []() -> std::vector<size_t> { throw test_exception("failure"); });

    sut = when_all(make_executor<1>(),
                   [](int x1, double x2, const std::string& x3, const std::vector<size_t>& x4) {
                       std::stringstream st;
                       st << x1 << " " << x2 << " " << x3 << " " << x4[0] << " " << x4[1];
                       return st.str();
                   },
                   f1, f2, f3, f4);

    wait_until_future_fails<test_exception>(sut);

    check_failure<test_exception>(sut, "failure");
    BOOST_REQUIRE_LE(4, custom_scheduler<0>::usage_counter());
    BOOST_REQUIRE_LE(1, custom_scheduler<1>::usage_counter());
}

BOOST_AUTO_TEST_SUITE_END()
