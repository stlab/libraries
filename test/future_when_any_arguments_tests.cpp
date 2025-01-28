/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#include <cstddef>
#include <exception>
#include <mutex>
#include <string_view>
#include <utility>

#include <boost/test/unit_test.hpp>

#include <stlab/concurrency/await.hpp>
#include <stlab/concurrency/future.hpp>
#include <stlab/test/model.hpp>

#include "future_test_helper.hpp"
#include "stlab/concurrency/default_executor.hpp"
#include "stlab/concurrency/immediate_executor.hpp"

using namespace stlab;
using namespace future_test_helper;

using lock_t = std::unique_lock<std::mutex>;

BOOST_FIXTURE_TEST_SUITE(future_when_any_arg_void, test_fixture<void>)
BOOST_AUTO_TEST_CASE(future_when_any_with_one_argument) {
    BOOST_TEST_MESSAGE("running future when_any one argument");

    auto [p0, f0] = package<int()>(immediate_executor, [] { return 0; });
    auto result = when_any(immediate_executor, when_any_result, std::move(f0));

    BOOST_REQUIRE(!result.is_ready());
    default_executor(std::move(p0));
    BOOST_REQUIRE((std::pair{0, 0} == await(std::move(result))));
}

BOOST_AUTO_TEST_CASE(future_when_any_multiple_arguments_first_succeeds) {
    BOOST_TEST_MESSAGE("running future when_any with multiple arguments and the first succeeds");

    auto [p0, f0] = package<int()>(immediate_executor, [] { return 0; });
    auto [p1, f1] = package<int()>(immediate_executor, [] { return 1; });
    auto [p2, f2] = package<int()>(immediate_executor, [] { return 2; });
    auto [p3, f3] = package<int()>(immediate_executor, [] { return 3; });

    auto result = when_any(immediate_executor, when_any_result, std::move(f0), std::move(f1),
                           std::move(f2), std::move(f3));

    BOOST_REQUIRE(!result.is_ready());
    default_executor(std::move(p0));
    BOOST_REQUIRE((std::pair{0, 0} == await(std::move(result))));
}

BOOST_AUTO_TEST_CASE(future_when_any_multiple_arguments_third_succeeds) {
    BOOST_TEST_MESSAGE("running future when_any with multiple arguments and the third succeeds");

    auto [p0, f0] = package<int()>(immediate_executor, [] { return 0; });
    auto [p1, f1] = package<int()>(immediate_executor, [] { return 1; });
    auto [p2, f2] = package<int()>(immediate_executor, [] { return 2; });
    auto [p3, f3] = package<int()>(immediate_executor, [] { return 3; });

    auto result = when_any(immediate_executor, when_any_result, std::move(f0), std::move(f1),
                           std::move(f2), std::move(f3));

    BOOST_REQUIRE(!result.is_ready());
    default_executor(std::move(p2));
    BOOST_REQUIRE((std::pair{2, 2} == await(std::move(result))));
}

BOOST_AUTO_TEST_CASE(future_when_any_one_succeeds_all_others_fail) {
    BOOST_TEST_MESSAGE("running future when_any one succeeds and all others fail");

    auto [p0, f0] = package<int()>(immediate_executor, [] { return 0; });
    auto [p1, f1] = package<int()>(immediate_executor, [] { return 1; });
    auto [p2, f2] = package<int()>(immediate_executor, [] { return 2; });
    auto [p3, f3] = package<int()>(immediate_executor, [] { return 3; });

    auto result = when_any(immediate_executor, when_any_result, std::move(f0), std::move(f1),
                           std::move(f2), std::move(f3));

    p0.set_exception(std::make_exception_ptr(test_exception("failure")));
    p1.set_exception(std::make_exception_ptr(test_exception("failure")));
    p3.set_exception(std::make_exception_ptr(test_exception("failure")));
    BOOST_REQUIRE(!result.is_ready());
    default_executor(std::move(p2));
    BOOST_REQUIRE((std::pair{2, 2} == await(std::move(result))));
}

BOOST_AUTO_TEST_CASE(future_when_any_all_fail) {
    BOOST_TEST_MESSAGE("running future when_any all fail");
    auto [p0, f0] = package<int()>(immediate_executor, [] { return 0; });
    auto [p1, f1] = package<int()>(immediate_executor, [] { return 1; });
    auto [p2, f2] = package<int()>(immediate_executor, [] { return 2; });
    auto [p3, f3] = package<int()>(immediate_executor, [] { return 3; });

    auto result = when_any(immediate_executor, when_any_result, std::move(f0), std::move(f1),
                           std::move(f2), std::move(f3));

    p0.set_exception(std::make_exception_ptr(test_exception("failure")));
    p1.set_exception(std::make_exception_ptr(test_exception("failure")));
    p2.set_exception(std::make_exception_ptr(test_exception("failure")));
    BOOST_REQUIRE(!result.is_ready());
    p3.set_exception(std::make_exception_ptr(test_exception("failure-final")));
    BOOST_REQUIRE(result.is_ready());
    BOOST_REQUIRE_EXCEPTION((void)result.get_try(), test_exception, ([](const auto& e) {
                                return std::string_view(e.what()) == "failure-final";
                            }));
}

/*
      /  f1  \
     / / f2 \ \
start           sut
     \ \ f3 / /
      \  f4  /
*/
BOOST_AUTO_TEST_CASE(future_when_any_int_arguments_with_diamond_formation_arguments) {
    BOOST_TEST_MESSAGE("running future when_any int arguments with diamond formation");

    auto [initial_promise, start] = package<int()>(immediate_executor, [] { return 42; });
    auto f1 = start | [](int x) { return x + 1; };
    auto f2 = start | [](int x) { return x + 2; };
    auto f3 = start | [](int x) { return x + 3; };
    auto f4 = start | [](int x) { return x + 4; };

    auto result = when_any(immediate_executor, when_any_result, std::move(f1), std::move(f2),
                           std::move(f3), std::move(f4));

    BOOST_REQUIRE(!result.is_ready());
    default_executor(std::move(initial_promise));
    BOOST_REQUIRE((std::pair{43, 0} == await(std::move(result))));
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(future_when_any_argument_move_only, test_fixture<move_only>)
BOOST_AUTO_TEST_CASE(future_when_any_move_only_arguments) {
    BOOST_TEST_MESSAGE("running future when_any with move-only arguments");

    auto [p0, f0] = package<move_only()>(immediate_executor, [] { return move_only(0); });
    auto [p1, f1] = package<move_only()>(immediate_executor, [] { return move_only(1); });

    auto result = when_any(immediate_executor, when_any_result, std::move(f0), std::move(f1));

    BOOST_REQUIRE(!result.is_ready());
    default_executor(std::move(p0));
    BOOST_REQUIRE((std::pair{move_only(0), 0} == await(std::move(result))));
}

BOOST_AUTO_TEST_SUITE_END()
