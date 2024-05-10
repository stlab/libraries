/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#include <atomic>
#include <chrono>
#include <cstddef>
#include <functional>
#include <iostream>
#include <string>
#include <thread>
#include <utility>

#include <boost/mpl/list.hpp>
#include <boost/test/unit_test.hpp>

#include <stlab/concurrency/await.hpp>
#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/future.hpp>
#include <stlab/concurrency/immediate_executor.hpp>
#include <stlab/concurrency/ready_future.hpp>
#include <stlab/test/model.hpp>
#include <stlab/utility.hpp>

#include "future_test_helper.hpp"

using namespace std;
using namespace stlab;
using namespace future_test_helper;

/**************************************************************************************************/

template <class T>
inline auto promise_future() {
    return package<T(T)>(immediate_executor,
                         [](auto&& x) -> decltype(x) { return std::forward<decltype(x)>(x); });
}

BOOST_AUTO_TEST_CASE(rvalue_through_continuation) {
    BOOST_TEST_MESSAGE("running passing rvalue to continuation");

    annotate_counters counters;

    auto pf = promise_future<annotate>();
    pf.first(annotate(counters));
    (void)pf.second.then([](const annotate&) {}); // copy happens here!

    std::cout << counters;
}

BOOST_AUTO_TEST_CASE(async_lambda_arguments) {
    {
        BOOST_TEST_MESSAGE("running async lambda argument of type rvalue -> value");

        annotate_counters counters;
        (void)async(immediate_executor, [](const annotate&) {}, annotate(counters));
        BOOST_REQUIRE(counters.remaining() == 0);
        BOOST_REQUIRE(counters._copy_ctor == 0);
    }

#if 0
    // These test disabled because clang-tidy insists on rewriting the by argument value to const&.
    {
        BOOST_TEST_MESSAGE("running async lambda argument of type lvalue -> value");

        annotate_counters counters;
        annotate const x(counters);
        (void)async(immediate_executor, [](const annotate&) {}, x);
        BOOST_REQUIRE(counters.remaining() == 1);
        BOOST_REQUIRE(counters._copy_ctor == 1);
    }

    {
        BOOST_TEST_MESSAGE("running async lambda argument of type ref -> value");

        annotate_counters counters;
        annotate x(counters);
        (void)async(immediate_executor, [](const annotate&) {}, std::ref(x));
        BOOST_REQUIRE(counters.remaining() == 1);
        BOOST_REQUIRE(counters._copy_ctor == 1);
    }

    {
        BOOST_TEST_MESSAGE("running async lambda argument of type cref -> value");

        annotate_counters counters;
        annotate const x(counters);
        (void)async(immediate_executor, [](const annotate&) {}, std::cref(x));
        BOOST_REQUIRE(counters.remaining() == 1);
        BOOST_REQUIRE(counters._copy_ctor == 1);
    }
#endif
//-------
#if 0
    {
    // EXPECTED WILL NOT COMPILE
        BOOST_TEST_MESSAGE("running async lambda argument of type rvalue -> &");

        annotate_counters counters;
        async(immediate_executor, [](annotate&){ }, annotate(counters));
        BOOST_REQUIRE(counters.remaining() == 0);
        BOOST_REQUIRE(counters._copy_ctor == 0);
    }

    {
        BOOST_TEST_MESSAGE("running async lambda argument of type lvalue -> &");

        annotate_counters counters;
        annotate x(counters);
        async(immediate_executor, [](annotate&){ }, x);
        BOOST_REQUIRE(counters.remaining() == 1);
        BOOST_REQUIRE(counters._copy_ctor == 1);
    }
#endif

    {
        BOOST_TEST_MESSAGE("running async lambda argument of type ref -> &");

        annotate_counters counters;
        annotate x(counters);
        (void)async(immediate_executor, [](annotate&) {}, std::ref(x));
        BOOST_REQUIRE(counters.remaining() == 1);
        BOOST_REQUIRE(counters._copy_ctor == 0);
    }

#if 0
    // EXPECTED WILL NOT COMPILE
    {
    BOOST_TEST_MESSAGE("running async lambda argument of type cref -> &");

    annotate_counters counters;
    annotate x(counters);
    async(immediate_executor, [](annotate&){ }, std::cref(x));
    BOOST_REQUIRE(counters.remaining() == 1);
    BOOST_REQUIRE(counters._copy_ctor == 1);
    }
#endif
    //-------
    {
        BOOST_TEST_MESSAGE("running async lambda argument of type rvalue -> const&");

        annotate_counters counters;
        (void)async(immediate_executor, [](const annotate&) {}, annotate(counters));
        BOOST_REQUIRE(counters.remaining() == 0);
        BOOST_REQUIRE(counters._copy_ctor == 0);
    }

    {
        BOOST_TEST_MESSAGE("running async lambda argument of type lvalue -> const&");

        annotate_counters counters;
        annotate const x(counters);
        (void)async(immediate_executor, [](const annotate&) {}, x);
        BOOST_REQUIRE(counters.remaining() == 1);
        BOOST_REQUIRE(counters._copy_ctor == 1);
    }

    {
        BOOST_TEST_MESSAGE("running async lambda argument of type ref -> const&");

        annotate_counters counters;
        annotate x(counters);
        (void)async(immediate_executor, [](const annotate&) {}, std::ref(x));
        BOOST_REQUIRE(counters.remaining() == 1);
        BOOST_REQUIRE(counters._copy_ctor == 0);
    }

    {
        BOOST_TEST_MESSAGE("running async lambda argument of type cref -> const&");

        annotate_counters counters;
        annotate const x(counters);
        (void)async(immediate_executor, [](const annotate&) {}, std::cref(x));
        BOOST_REQUIRE(counters.remaining() == 1);
        BOOST_REQUIRE(counters._copy_ctor == 0);
    }
    //-------
    {
        BOOST_TEST_MESSAGE("running async lambda argument of type rvalue -> &&");

        annotate_counters counters;
        (void)async(immediate_executor, [](annotate&&) {}, annotate(counters));
        BOOST_REQUIRE(counters.remaining() == 0);
        BOOST_REQUIRE(counters._copy_ctor == 0);
    }

    {
        BOOST_TEST_MESSAGE("running async lambda argument of type lvalue -> &&");

        annotate_counters counters;
        annotate const x(counters);
        (void)async(immediate_executor, [](annotate&&) {}, x);
        BOOST_REQUIRE(counters.remaining() == 1);
        BOOST_REQUIRE(counters._copy_ctor == 1);
    }

#if 0
    // EXPECTED WILL NOT COMPILE
    {
    BOOST_TEST_MESSAGE("running async lambda argument of type ref -> &&");

    annotate_counters counters;
    annotate x(counters);
    async(immediate_executor, [](annotate&&){ }, std::ref(x));
    BOOST_REQUIRE(counters.remaining() == 1);
    BOOST_REQUIRE(counters._copy_ctor == 0);
    }

    {
    BOOST_TEST_MESSAGE("running async lambda argument of type cref -> &&");

    annotate_counters counters;
    annotate x(counters);
    async(immediate_executor, [](annotate&&){ }, std::cref(x));
    BOOST_REQUIRE(counters.remaining() == 1);
    BOOST_REQUIRE(counters._copy_ctor == 0);
    }
#endif
}

/**************************************************************************************************/

using all_test_types = boost::mpl::list<void, int, stlab::move_only>;

BOOST_AUTO_TEST_CASE_TEMPLATE(future_default_constructed, T, all_test_types) {
    BOOST_TEST_MESSAGE("running future default constructed of type " << typeid(T).name());

    auto sut = future<T>();
    BOOST_REQUIRE(sut.valid() == false);
    BOOST_REQUIRE(sut.is_ready() == false);
}

using copyable_test_types = boost::mpl::list<int, double>;

BOOST_AUTO_TEST_CASE_TEMPLATE(future_constructed_minimal_fn, T, copyable_test_types) {
    BOOST_TEST_MESSAGE("running future with minimal copyable type " << typeid(T).name());

    test_setup const setup;
    {
        auto sut = async(make_executor<0>(), []() -> T { return T(0); });
        BOOST_REQUIRE(sut.valid() == true);

        sut.reset();
        BOOST_REQUIRE(sut.valid() == false);
        BOOST_REQUIRE(sut.is_ready() == false);
    }
    BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(future_constructed_minimal_fn_with_parameters,
                              T,
                              copyable_test_types) {
    BOOST_TEST_MESSAGE("running future with minimal copyable type and passed parameter "
                       << typeid(T).name());

    test_setup const setup;
    {
        auto sut = async(make_executor<0>(), [](auto x) -> T { return x + T(0); }, T(42));
        BOOST_REQUIRE(sut.valid() == true);

        auto result = await(std::move(sut));

        BOOST_WARN_EQUAL(T(42) + T(0), result);
    }
    BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
}

BOOST_AUTO_TEST_CASE(future_constructed_minimal_fn_moveonly) {
    BOOST_TEST_MESSAGE("running future with move only type");

    test_setup const setup;
    {
        auto sut =
            async(make_executor<0>(), []() -> stlab::move_only { return stlab::move_only{42}; });
        BOOST_REQUIRE(sut.valid() == true);

        auto result = await(std::move(sut));

        BOOST_REQUIRE_EQUAL(42, result.member());
    }
    BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
}

BOOST_AUTO_TEST_CASE(future_equality_tests) {
    BOOST_TEST_MESSAGE("running future equality tests");
    {
        future<int> const a;
        future<int> const b;
        BOOST_REQUIRE(a == b);
        BOOST_REQUIRE(!(a != b));
    }

    {
        future<void> const a;
        future<void> const b;
        BOOST_REQUIRE(a == b);
        BOOST_REQUIRE(!(a != b));
    }

    {
        future<move_only> const a;
        future<move_only> const b;
        BOOST_REQUIRE(a == b);
        BOOST_REQUIRE(!(a != b));
    }

    {
        future<int> const a = async(default_executor, [] { return 42; });
        const auto& b = a;
        BOOST_REQUIRE(a == b);
    }
    {
        future<void> const a = async(default_executor, [] {});
        const auto& b = a;
        BOOST_REQUIRE(a == b);
    }

    {
        future<int> const a = async(default_executor, [] { return 42; });
        future<int> const b = async(default_executor, [] { return 42; });
        BOOST_REQUIRE(a != b);
    }
    {
        future<void> const a = async(default_executor, [] {});
        future<void> const b = async(default_executor, [] {});
        BOOST_REQUIRE(a != b);
    }
    {
        future<move_only> const a = async(default_executor, [] { return move_only(42); });
        future<move_only> const b;
        BOOST_REQUIRE(a != b);
    }
}

BOOST_AUTO_TEST_CASE(future_swap_tests) {
    {
        auto a = package<int(int)>(immediate_executor, [](int a) { return a + 2; });
        auto b = package<int(int)>(immediate_executor, [](int a) { return a + 4; });

        std::swap(a, b);

        a.first(1);
        b.first(2);

        BOOST_REQUIRE_EQUAL(5, *a.second.get_try());
        BOOST_REQUIRE_EQUAL(4, *b.second.get_try());
    }
    {
        int x(0);
        int y(0);
        auto a = package<void(int)>(immediate_executor, [&x](int a) { x = a + 2; });
        auto b = package<void(int)>(immediate_executor, [&y](int a) { y = a + 4; });

        std::swap(a, b);

        a.first(1);
        b.first(2);

        BOOST_REQUIRE_EQUAL(5, y);
        BOOST_REQUIRE_EQUAL(4, x);
    }
    {
        auto a =
            package<move_only(int)>(immediate_executor, [](int a) { return move_only(a + 2); });
        auto b =
            package<move_only(int)>(immediate_executor, [](int a) { return move_only(a + 4); });

        std::swap(a, b);

        a.first(1);
        b.first(2);

        BOOST_REQUIRE_EQUAL(5, a.second.get_try()->member());
        BOOST_REQUIRE_EQUAL(4, b.second.get_try()->member());
    }
}

BOOST_FIXTURE_TEST_SUITE(future_then_void, test_fixture<int>)

BOOST_AUTO_TEST_CASE(future_get_try_refref) {
    BOOST_TEST_MESSAGE("future get_try()&& accessor test");

    sut = async(default_executor, [] {
              return 42;
          }).then([](int) -> int {
                throw test_exception("failure");
            }).recover([](auto&& f) {
        try {
            std::forward<decltype(f)>(f).get_try();
            return 0;
        } catch (const test_exception&) {
            return 42;
        }
    });

    wait_until_future_completed(copy(sut));
    BOOST_REQUIRE_EQUAL(42, *sut.get_try());
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_CASE(future_wait_copyable_value) {
    BOOST_TEST_MESSAGE("future wait with copyable value");
    auto answer = [] { return 42; };

    stlab::future<int> f = stlab::async(stlab::default_executor, answer);

    BOOST_REQUIRE_EQUAL(42, stlab::await(std::move(f)));
}

BOOST_AUTO_TEST_CASE(future_wait_moveonly_value) {
    BOOST_TEST_MESSAGE("future wait with moveonly value");
    auto answer = [] { return stlab::move_only(42); };

    stlab::future<stlab::move_only> f = stlab::async(stlab::default_executor, answer);

    BOOST_REQUIRE_EQUAL(42, stlab::await(std::move(f)).member());
}

BOOST_AUTO_TEST_CASE(future_wait_void) {
    BOOST_TEST_MESSAGE("future wait with void");
    int v = 0;
    auto answer = [&] { v = 42; };

    stlab::future<void> f = stlab::async(stlab::default_executor, answer);

    stlab::await(std::move(f));
    BOOST_REQUIRE_EQUAL(42, v);
}

BOOST_AUTO_TEST_CASE(future_wait_void_with_timeout) {
    BOOST_TEST_MESSAGE("future wait with void");
    int v = 0;
    auto answer = [&] { v = 42; };

    stlab::future<void> f = stlab::async(stlab::default_executor, answer);

    auto r = stlab::await_for(std::move(f), std::chrono::seconds(2));
    BOOST_REQUIRE_EQUAL(42, v);
    BOOST_REQUIRE_EQUAL(true, r.is_ready());
}

BOOST_AUTO_TEST_CASE(future_wait_void_with_timeout_reached) {
    BOOST_TEST_MESSAGE("future wait with void with a timeout");
    auto answer = [&] {
        invoke_waiting([] { std::this_thread::sleep_for(std::chrono::milliseconds(500)); });
    };

    stlab::future<void> f = stlab::async(stlab::default_executor, answer);
    f = stlab::await_for(std::move(f), std::chrono::milliseconds(100));
    BOOST_REQUIRE(!f.is_ready());
    stlab::await(std::move(f));
}

BOOST_AUTO_TEST_CASE(future_wait_int_with_timeout_reached) {
    BOOST_TEST_MESSAGE("future wait with int with a timeout");
    auto answer = [&] {
        stlab::invoke_waiting([] { std::this_thread::sleep_for(std::chrono::milliseconds(500)); });
        return 42;
    };

    stlab::future<int> f = stlab::async(stlab::default_executor, answer);
    f = stlab::await_for(std::move(f), std::chrono::milliseconds(100));
    BOOST_REQUIRE(!f.is_ready());
    BOOST_REQUIRE(stlab::await(std::move(f)) == 42);
}

BOOST_AUTO_TEST_CASE(future_wait_move_only_with_timeout_reached) {
    BOOST_TEST_MESSAGE("future wait with move_only with a timeout");
    auto answer = [&] {
        stlab::invoke_waiting([] { std::this_thread::sleep_for(std::chrono::milliseconds(500)); });
        return move_only(42);
    };

    stlab::future<move_only> f = stlab::async(stlab::default_executor, answer);
    f = stlab::await_for(std::move(f), std::chrono::milliseconds(100));
    BOOST_REQUIRE(!f.is_ready());
    BOOST_REQUIRE(stlab::await(std::move(f)).member() == 42);
}

BOOST_AUTO_TEST_CASE(future_wait_copyable_value_error_case) {
    BOOST_TEST_MESSAGE("future wait with copyable value error case");
    auto answer = []() -> int { throw test_exception("failure"); };

    stlab::future<int> f = stlab::async(stlab::default_executor, answer);

    BOOST_REQUIRE_EXCEPTION(stlab::await(std::move(f)), test_exception, ([](const auto& e) {
                                return std::string(e.what()) == std::string("failure");
                            }));
}

BOOST_AUTO_TEST_CASE(future_wait_moveonly_value_error_case) {
    BOOST_TEST_MESSAGE("future wait with moveonly value");
    auto answer = []() -> stlab::move_only { throw test_exception("failure"); };

    stlab::future<stlab::move_only> f = stlab::async(stlab::default_executor, answer);

    BOOST_REQUIRE_EXCEPTION(stlab::await(std::move(f)), test_exception, ([](const auto& e) {
                                return std::string(e.what()) == std::string("failure");
                            }));
}

BOOST_AUTO_TEST_CASE(future_wait_void_error_case) {
    BOOST_TEST_MESSAGE("future wait with void error case");

    auto answer = [] { throw test_exception("failure"); };

    stlab::future<void> f = stlab::async(stlab::default_executor, answer);

    BOOST_REQUIRE_EXCEPTION(stlab::await(std::move(f)), test_exception, ([](const auto& e) {
                                return std::string(e.what()) == std::string("failure");
                            }));
}

BOOST_AUTO_TEST_CASE(future_wait_void_error_case_with_timeout) {
    BOOST_TEST_MESSAGE("future wait with void error case with timout");

    auto answer = [] { throw test_exception("failure"); };

    stlab::future<void> f = stlab::async(stlab::default_executor, answer);

    BOOST_REQUIRE_EXCEPTION(
        stlab::await_for(std::move(f), std::chrono::seconds(60)).get_try(), test_exception,
        ([](const auto& e) { return std::string(e.what()) == std::string("failure"); }));
}

BOOST_AUTO_TEST_CASE(future_wait_copyable_value_timeout) {
    BOOST_TEST_MESSAGE("future wait with copyable value with timeout");
    auto answer = [] {
        stlab::invoke_waiting([] { std::this_thread::sleep_for(std::chrono::seconds(1)); });
        return 42;
    };

    stlab::future<int> f = stlab::async(stlab::default_executor, answer);

    auto r = stlab::await_for(std::move(f), std::chrono::milliseconds(500));
    // timeout should have been reached
    BOOST_REQUIRE(!r.is_ready());
}

BOOST_AUTO_TEST_CASE(future_wait_moveonly_value_and_timeout) {
    BOOST_TEST_MESSAGE("future wait with moveonly value");
    auto answer = [] {
        stlab::invoke_waiting([] { std::this_thread::sleep_for(std::chrono::milliseconds(1)); });
        return stlab::move_only(42);
    };

    stlab::future<stlab::move_only> f = stlab::async(stlab::default_executor, answer);
    auto r = stlab::await_for(std::move(f), std::chrono::milliseconds(500));
    BOOST_REQUIRE(r.is_ready());
    BOOST_REQUIRE_EQUAL(42, r.get_try()->member());
}

BOOST_AUTO_TEST_CASE(future_wait_moveonly_value_error_case_and_timeout) {
    BOOST_TEST_MESSAGE("future wait with moveonly value and timeout set");
    auto answer = [] {
        throw test_exception("failure");
        return stlab::move_only(42);
    };

    stlab::future<stlab::move_only> f = stlab::async(stlab::default_executor, answer);

    BOOST_REQUIRE_EXCEPTION(
        stlab::await_for(std::move(f), std::chrono::seconds(500)).get_try(), test_exception,
        ([](const auto& e) { return std::string(e.what()) == std::string("failure"); }));
}

BOOST_AUTO_TEST_CASE(future_int_detach_without_execution) {
    BOOST_TEST_MESSAGE("future int detach without execution");
    annotate_counters counter;
    bool check = true;
    {
        auto p = package<int()>(immediate_executor, [] { return 42; });
        p.second.then([a = annotate(counter), &_check = check](int) { _check = false; }).detach();
    }
    std::cout << counter;

    BOOST_REQUIRE_EQUAL(0, counter.remaining());
    BOOST_REQUIRE(check);
}

BOOST_AUTO_TEST_CASE(future_move_only_detach_without_execution) {
    BOOST_TEST_MESSAGE("future move_only detach without execution");
    annotate_counters counter;
    bool check = true;
    {
        auto p = package<move_only()>(immediate_executor, [] { return move_only{42}; });
        auto r = std::move(p.second).then(
            [a = annotate(counter), &_check = check](auto&&) { _check = false; });
        r.detach();
    }
    std::cout << counter;

    BOOST_REQUIRE_EQUAL(0, counter.remaining());
    BOOST_REQUIRE(check);
}

BOOST_AUTO_TEST_CASE(future_void_detach_without_execution) {
    BOOST_TEST_MESSAGE("future void detach without execution");
    annotate_counters counter;
    bool check = true;
    {
        auto p = package<void()>(immediate_executor, [] {});
        p.second.then([a = annotate(counter), &_check = check]() { _check = false; }).detach();
    }
    std::cout << counter;

    BOOST_REQUIRE_EQUAL(0, counter.remaining());
    BOOST_REQUIRE(check);
}

BOOST_AUTO_TEST_CASE(future_int_detach_with_execution) {
    BOOST_TEST_MESSAGE("future int detach with execution");
    annotate_counters counter;
    int result = 0;
    {
        auto p = package<int()>(stlab::immediate_executor, [] { return 42; });
        p.second.then([a = stlab::annotate(counter), &_result = result](int x) { _result = x; })
            .detach();
        p.first();
    }
    std::cout << counter;

    BOOST_REQUIRE_EQUAL(counter._dtor, counter._move_ctor + 1);
    BOOST_REQUIRE_EQUAL(42, result);
}

BOOST_AUTO_TEST_CASE(future_void_detach_with_execution) {
    BOOST_TEST_MESSAGE("future void detach with execution");
    annotate_counters counter;
    bool check = false;
    {
        auto p = package<void()>(stlab::immediate_executor, [] {});
        p.second.then([a = stlab::annotate(counter), &_check = check]() { _check = true; })
            .detach();
        p.first();
    }
    std::cout << counter;

    BOOST_REQUIRE_EQUAL(counter._dtor, counter._move_ctor + 1);
    BOOST_REQUIRE(check);
}

BOOST_AUTO_TEST_CASE(future_move_only_detach_with_execution) {
    BOOST_TEST_MESSAGE("future move_only detach with execution");
    annotate_counters counter;
    int result = 0;
    {
        auto p = package<move_only()>(stlab::immediate_executor, [] { return move_only{42}; });
        auto r = std::move(p.second).then(
            [a = stlab::annotate(counter), &_result = result](auto&& x) { _result = x.member(); });
        r.detach();
        p.first();
    }
    std::cout << counter;

    BOOST_REQUIRE_EQUAL(counter._dtor, counter._move_ctor + 1);
    BOOST_REQUIRE_EQUAL(42, result);
}

BOOST_AUTO_TEST_CASE(future_reduction_with_mutable_task) {
    BOOST_TEST_MESSAGE("future reduction with mutable task");

    auto func = [i = 0]() mutable {
        i++;
        return i;
    };

    auto result = stlab::async(stlab::default_executor, [func = std::move(func)]() mutable {
        func();

        return stlab::async(stlab::default_executor,
                            [func = std::move(func)]() mutable { return func(); });
    });

    BOOST_REQUIRE_EQUAL(2, stlab::await(std::move(result)));
}

BOOST_AUTO_TEST_CASE(future_reduction_with_mutable_void_task) {
    BOOST_TEST_MESSAGE("future reduction with mutable task");

    std::atomic_int check{0};
    auto func = [&check]() mutable { ++check; };

    auto result = stlab::async(stlab::default_executor, [func = std::move(func)]() mutable {
        func();

        return stlab::async(stlab::default_executor,
                            [func = std::move(func)]() mutable { func(); });
    });

    static_cast<void>(stlab::await(std::move(result)));

    BOOST_REQUIRE_EQUAL(2, check);
}

BOOST_AUTO_TEST_CASE(future_reduction_with_move_only_mutable_task) {
    BOOST_TEST_MESSAGE("future reduction with move only mutable task");

    auto func = [i = move_only{0}]() mutable {
        i = move_only{i.member() + 1};
        return std::move(i);
    };

    auto result = stlab::async(stlab::default_executor, [func = std::move(func)]() mutable {
        func();

        return stlab::async(stlab::default_executor,
                            [func = std::move(func)]() mutable { return func(); });
    });

    BOOST_REQUIRE_EQUAL(2, stlab::await(std::move(result)).member());
}

BOOST_AUTO_TEST_CASE(future_reduction_with_move_only_mutable_void_task) {
    BOOST_TEST_MESSAGE("future reduction with move only mutable void task");

    int check{0};
    auto func = [i = move_only{0}, &check]() mutable {
        i = move_only{i.member() + 1};
        check += i.member();
    };

    auto result = stlab::async(stlab::default_executor, [func = std::move(func)]() mutable {
        func();

        return stlab::async(stlab::default_executor,
                            [func = std::move(func)]() mutable { return func(); });
    });

    static_cast<void>(stlab::await(std::move(result)));

    BOOST_REQUIRE_EQUAL(3, check);
}

BOOST_AUTO_TEST_CASE(future_reduction_executor) {
    BOOST_TEST_MESSAGE("future reduction executor should propagate from outermost future");

    size_t outer_count{0};
    size_t inner_count{0};
    auto outer_executor{[&](auto&& f) {
        ++outer_count;
        std::forward<decltype(f)>(f)();
    }};
    auto inner_executor{[&](auto&& f) {
        ++inner_count;
        std::forward<decltype(f)>(f)();
    }};

    auto f = make_ready_future(5, outer_executor) |
             [&](int x) { return make_ready_future(x, inner_executor); };

    BOOST_REQUIRE_EQUAL(1, outer_count);
    BOOST_REQUIRE_EQUAL(0, inner_count);

    auto f1 = f | [](int x) { return x; };

    BOOST_REQUIRE_EQUAL(2, outer_count);
    BOOST_REQUIRE_EQUAL(0, inner_count);
    BOOST_REQUIRE_EQUAL(5, *f1.get_try());
}
