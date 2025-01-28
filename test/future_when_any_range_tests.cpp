/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#include <array>
#include <cstddef>
#include <exception>
#include <mutex>
#include <optional>
#include <string_view>
#include <utility>

#include <boost/test/unit_test.hpp>

#include <stlab/concurrency/await.hpp>
#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/future.hpp>
#include <stlab/concurrency/ready_future.hpp>
#include <stlab/test/model.hpp>

#include "future_test_helper.hpp"
#include "stlab/concurrency/immediate_executor.hpp"

using namespace stlab;
using namespace future_test_helper;

using lock_t = std::unique_lock<std::mutex>;

template <class T>
auto range_pair(T& r) {
    return std::pair{r.begin(), r.end()};
}

BOOST_FIXTURE_TEST_SUITE(future_when_any_range_void, test_fixture<void>)
BOOST_AUTO_TEST_CASE(future_when_any_void_void_empty_range) {
    BOOST_TEST_MESSAGE("running future when_any void void with empty range");

    auto result = when_any(immediate_executor, when_any_result,
                           std::pair<future<void>*, future<void>*>{nullptr, nullptr});
    BOOST_REQUIRE_EXCEPTION((void)result.get_try(), future_error, [](const auto& e) {
        return e.code() == future_error_codes::broken_promise;
    });
}

BOOST_AUTO_TEST_CASE(future_when_any_int_void_empty_range) {
    BOOST_TEST_MESSAGE("running future when_any int void with empty range");

    auto result = when_any(immediate_executor, when_any_result,
                           std::pair<future<int>*, future<int>*>{nullptr, nullptr});
    BOOST_REQUIRE_EXCEPTION((void)result.get_try(), future_error, [](const auto& e) {
        return e.code() == future_error_codes::broken_promise;
    });
}

BOOST_AUTO_TEST_CASE(future_when_any_void_void_range_with_one_element) {
    BOOST_TEST_MESSAGE("running future when_any void void with one element");

    auto [p0, f0] = package<void()>(immediate_executor, [] {});
    auto result = when_any(immediate_executor, when_any_result, std::pair{&f0, &f0 + 1});

    BOOST_REQUIRE(!result.is_ready());
    default_executor(std::move(p0));
    BOOST_REQUIRE((std::size_t{0} == await(std::move(result))));
}

BOOST_AUTO_TEST_CASE(future_when_any_int_void_range_with_one_element) {
    BOOST_TEST_MESSAGE("running future when_any int void with range of one element");

    auto [p0, f0] = package<int()>(immediate_executor, [] { return 0; });
    auto result = when_any(immediate_executor, when_any_result, std::pair{&f0, &f0 + 1});

    BOOST_REQUIRE(!result.is_ready());
    default_executor(std::move(p0));
    BOOST_REQUIRE((std::pair{0, 0} == await(std::move(result))));
}

BOOST_AUTO_TEST_CASE(future_when_any_int_void_range_with_many_elements_first_succeeds) {
    BOOST_TEST_MESSAGE(
        "running future when_any int void with range with many elements and the first succeeds");

    auto [p0, f0] = package<int()>(immediate_executor, [] { return 0; });
    auto [p1, f1] = package<int()>(immediate_executor, [] { return 1; });
    auto [p2, f2] = package<int()>(immediate_executor, [] { return 2; });
    auto [p3, f3] = package<int()>(immediate_executor, [] { return 3; });

    std::array a{std::move(f0), std::move(f1), std::move(f2), std::move(f3)};

    auto result = when_any(immediate_executor, when_any_result, range_pair(a));

    BOOST_REQUIRE(!result.is_ready());
    default_executor(std::move(p0));
    BOOST_REQUIRE((std::pair{0, 0} == await(std::move(result))));
}

BOOST_AUTO_TEST_CASE(future_when_any_int_void_range_with_many_elements_middle_succeeds) {
    BOOST_TEST_MESSAGE(
        "running future when_any int void with range with many elements and one in the middle succeeds");

    auto [p0, f0] = package<int()>(immediate_executor, [] { return 0; });
    auto [p1, f1] = package<int()>(immediate_executor, [] { return 1; });
    auto [p2, f2] = package<int()>(immediate_executor, [] { return 2; });
    auto [p3, f3] = package<int()>(immediate_executor, [] { return 3; });

    std::array a{std::move(f0), std::move(f1), std::move(f2), std::move(f3)};

    auto result = when_any(immediate_executor, when_any_result, range_pair(a));

    BOOST_REQUIRE(!result.is_ready());
    default_executor(std::move(p2));
    BOOST_REQUIRE((std::pair{2, 2} == await(std::move(result))));
}

BOOST_AUTO_TEST_CASE(
    future_when_any_int_void_range_with_many_elements_one_succeeds_all_other_fails) {
    BOOST_TEST_MESSAGE(
        "running future when_any int void with range with many elements and one succeeds all other fails");

    auto [p0, f0] = package<int()>(immediate_executor, [] { return 0; });
    auto [p1, f1] = package<int()>(immediate_executor, [] { return 1; });
    auto [p2, f2] = package<int()>(immediate_executor, [] { return 2; });
    auto [p3, f3] = package<int()>(immediate_executor, [] { return 3; });

    std::array a{std::move(f0), std::move(f1), std::move(f2), std::move(f3)};

    auto result = when_any(immediate_executor, when_any_result, range_pair(a));

    p0.set_exception(std::make_exception_ptr(test_exception("failure")));
    p1.set_exception(std::make_exception_ptr(test_exception("failure")));
    p3.set_exception(std::make_exception_ptr(test_exception("failure")));
    BOOST_REQUIRE(!result.is_ready());
    default_executor(std::move(p2));
    BOOST_REQUIRE((std::pair{2, 2} == await(std::move(result))));
}

BOOST_AUTO_TEST_CASE(future_when_any_void_all_are_ready_at_the_beginning) {
    BOOST_TEST_MESSAGE("running future when_any void when all futures are ready from the start");

    std::array a{make_ready_future(immediate_executor), make_ready_future(immediate_executor)};

    auto result = when_any(immediate_executor, when_any_result, range_pair(a));

    BOOST_REQUIRE((0 == *result.get_try()));
}

BOOST_AUTO_TEST_CASE(future_when_any_int_void_range_with_many_elements_all_fails) {
    BOOST_TEST_MESSAGE("running future when_any void with range all fails");

    std::array a{make_exceptional_future<void>(std::make_exception_ptr(test_exception("failure")),
                                               immediate_executor),
                 make_exceptional_future<void>(
                     std::make_exception_ptr(test_exception("failure-final")), immediate_executor)};

    auto result = when_any(immediate_executor, when_any_result, range_pair(a));

    BOOST_REQUIRE(result.is_ready());
    BOOST_REQUIRE_EXCEPTION((void)result.get_try(), test_exception, ([](const auto& e) {
                                return std::string_view(e.what()) == "failure-final";
                            }));
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

    auto [initial_promise, start] = package<int()>(immediate_executor, [] { return 42; });
    auto f1 = start | [](int x) { return x + 1; };
    auto f2 = start | [](int x) { return x + 2; };
    auto f3 = start | [](int x) { return x + 3; };
    auto f4 = start | [](int x) { return x + 4; };

    std::array a{std::move(f1), std::move(f2), std::move(f3), std::move(f4)};
    auto result = when_any(immediate_executor, when_any_result, range_pair(a));

    BOOST_REQUIRE(!result.is_ready());
    default_executor(std::move(initial_promise));
    BOOST_REQUIRE((std::pair{43, 0} == await(std::move(result))));
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(future_when_any_range_move_only, test_fixture<stlab::move_only>)

BOOST_AUTO_TEST_CASE(future_when_any_range_move_only) {
    BOOST_TEST_MESSAGE("running future when_any rang with move-only arguments");

    auto [p0, f0] = package<move_only()>(immediate_executor, [] { return move_only(0); });
    auto [p1, f1] = package<move_only()>(immediate_executor, [] { return move_only(1); });

    std::array a{std::move(f0), std::move(f1)};

    auto result = when_any(immediate_executor, when_any_result, range_pair(a));

    BOOST_REQUIRE(!result.is_ready());
    default_executor(std::move(p0));
    BOOST_REQUIRE((std::pair{move_only(0), 0} == await(std::move(result))));
}

BOOST_AUTO_TEST_SUITE_END()
