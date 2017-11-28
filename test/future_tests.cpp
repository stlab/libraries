/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#include <boost/test/unit_test.hpp>
#include <boost/mpl/list.hpp>

#include <stlab/concurrency/concurrency.hpp>
#include <stlab/concurrency/immediate_executor.hpp>
#include <stlab/test/model.hpp>

#include "future_test_helper.hpp"

using namespace stlab;
using namespace future_test_helper;

/**************************************************************************************************/

template <class T>
inline auto promise_future() {
    return package<T(T)>(immediate_executor, [](auto&& x) -> decltype(x) { return std::forward<decltype(x)>(x); });
}


BOOST_AUTO_TEST_CASE(rvalue_through_continuation) {
    BOOST_TEST_MESSAGE("running passing rvalue to continuation");

    annotate_counters counters;

    auto pf = promise_future<annotate>();
    pf.first(annotate(counters));
    pf.second.then([](const annotate&){ }); // copy happens here!

    std::cout << counters;
}

BOOST_AUTO_TEST_CASE(async_lambda_arguments) {
    {
        BOOST_TEST_MESSAGE("running async lambda argument of type rvalue -> value");

        annotate_counters counters;
        async(immediate_executor, [](annotate x){ }, annotate(counters));
        BOOST_REQUIRE(counters.remaining() == 0);
        BOOST_REQUIRE(counters._copy_ctor == 0);
    }

    {
        BOOST_TEST_MESSAGE("running async lambda argument of type lvalue -> value");

        annotate_counters counters;
        annotate x(counters);
        async(immediate_executor, [](annotate x){ }, x);
        BOOST_REQUIRE(counters.remaining() == 1);
        BOOST_REQUIRE(counters._copy_ctor == 1);
    }

    {
        BOOST_TEST_MESSAGE("running async lambda argument of type ref -> value");

        annotate_counters counters;
        annotate x(counters);
        async(immediate_executor, [](annotate x){ }, std::ref(x));
        BOOST_REQUIRE(counters.remaining() == 1);
        BOOST_REQUIRE(counters._copy_ctor == 1);
    }

    {
        BOOST_TEST_MESSAGE("running async lambda argument of type cref -> value");

        annotate_counters counters;
        annotate x(counters);
        async(immediate_executor, [](annotate x){ }, std::cref(x));
        BOOST_REQUIRE(counters.remaining() == 1);
        BOOST_REQUIRE(counters._copy_ctor == 1);
    }
//-------
#if 0
    {
    // EXPECTED WILL NOT COMPILE
        BOOST_TEST_MESSAGE("running async lambda argument of type rvalue -> &");

        annotate_counters counters;
        async(immediate_executor, [](annotate& x){ }, annotate(counters));
        BOOST_REQUIRE(counters.remaining() == 0);
        BOOST_REQUIRE(counters._copy_ctor == 0);
    }

    {
        BOOST_TEST_MESSAGE("running async lambda argument of type lvalue -> &");

        annotate_counters counters;
        annotate x(counters);
        async(immediate_executor, [](annotate& x){ }, x);
        BOOST_REQUIRE(counters.remaining() == 1);
        BOOST_REQUIRE(counters._copy_ctor == 1);
    }
#endif

    {
        BOOST_TEST_MESSAGE("running async lambda argument of type ref -> &");

        annotate_counters counters;
        annotate x(counters);
        async(immediate_executor, [](annotate& x){ }, std::ref(x));
        BOOST_REQUIRE(counters.remaining() == 1);
        BOOST_REQUIRE(counters._copy_ctor == 0);
    }

#if 0
    // EXPECTED WILL NOT COMPILE
    {
    BOOST_TEST_MESSAGE("running async lambda argument of type cref -> &");

    annotate_counters counters;
    annotate x(counters);
    async(immediate_executor, [](annotate& x){ }, std::cref(x));
    BOOST_REQUIRE(counters.remaining() == 1);
    BOOST_REQUIRE(counters._copy_ctor == 1);
    }
#endif
//-------
    {
        BOOST_TEST_MESSAGE("running async lambda argument of type rvalue -> const&");

        annotate_counters counters;
        async(immediate_executor, [](const annotate& x){ }, annotate(counters));
        BOOST_REQUIRE(counters.remaining() == 0);
        BOOST_REQUIRE(counters._copy_ctor == 0);
    }

    {
        BOOST_TEST_MESSAGE("running async lambda argument of type lvalue -> const&");

        annotate_counters counters;
        annotate x(counters);
        async(immediate_executor, [](const annotate& x){ }, x);
        BOOST_REQUIRE(counters.remaining() == 1);
        BOOST_REQUIRE(counters._copy_ctor == 1);
    }

    {
        BOOST_TEST_MESSAGE("running async lambda argument of type ref -> const&");

        annotate_counters counters;
        annotate x(counters);
        async(immediate_executor, [](const annotate& x){ }, std::ref(x));
        BOOST_REQUIRE(counters.remaining() == 1);
        BOOST_REQUIRE(counters._copy_ctor == 0);
    }

    {
        BOOST_TEST_MESSAGE("running async lambda argument of type cref -> const&");

        annotate_counters counters;
        annotate x(counters);
        async(immediate_executor, [](const annotate& x){ }, std::cref(x));
        BOOST_REQUIRE(counters.remaining() == 1);
        BOOST_REQUIRE(counters._copy_ctor == 0);
    }
//-------
    {
        BOOST_TEST_MESSAGE("running async lambda argument of type rvalue -> &&");

        annotate_counters counters;
        async(immediate_executor, [](annotate&& x){ }, annotate(counters));
        BOOST_REQUIRE(counters.remaining() == 0);
        BOOST_REQUIRE(counters._copy_ctor == 0);
    }

    {
        BOOST_TEST_MESSAGE("running async lambda argument of type lvalue -> &&");

        annotate_counters counters;
        annotate x(counters);
        async(immediate_executor, [](annotate&& x){ }, x);
        BOOST_REQUIRE(counters.remaining() == 1);
        BOOST_REQUIRE(counters._copy_ctor == 1);
    }

#if 0
    // EXPECTED WILL NOT COMPILE
    {
    BOOST_TEST_MESSAGE("running async lambda argument of type ref -> &&");

    annotate_counters counters;
    annotate x(counters);
    async(immediate_executor, [](annotate&& x){ }, std::ref(x));
    BOOST_REQUIRE(counters.remaining() == 1);
    BOOST_REQUIRE(counters._copy_ctor == 0);
    }

    {
    BOOST_TEST_MESSAGE("running async lambda argument of type cref -> &&");

    annotate_counters counters;
    annotate x(counters);
    async(immediate_executor, [](annotate&& x){ }, std::cref(x));
    BOOST_REQUIRE(counters.remaining() == 1);
    BOOST_REQUIRE(counters._copy_ctor == 0);
    }
#endif
}

/**************************************************************************************************/

using all_test_types = boost::mpl::list<void, int, stlab::move_only>;

BOOST_AUTO_TEST_CASE_TEMPLATE(future_default_constructed, T, all_test_types)
{
    BOOST_TEST_MESSAGE("running future default constructed of type " << typeid(T).name());

    auto sut = future<T>();
    BOOST_REQUIRE(sut.valid() == false);
    BOOST_REQUIRE(sut.is_ready() == false);
}

using copyable_test_types = boost::mpl::list<int, double>;

BOOST_AUTO_TEST_CASE_TEMPLATE(future_constructed_minimal_fn, T, copyable_test_types) {
    BOOST_TEST_MESSAGE("running future with minimal copyable type " << typeid(T).name());

    test_setup setup;
    {
        auto sut = async(custom_scheduler<0>(), []()->T { return T(0); });
        BOOST_REQUIRE(sut.valid() == true);
        BOOST_REQUIRE(sut.error().is_initialized() == false);

        sut.reset();
        BOOST_REQUIRE(sut.valid() == false);
        BOOST_REQUIRE(sut.is_ready() == false);
    }
    BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
}

BOOST_AUTO_TEST_CASE_TEMPLATE(future_constructed_minimal_fn_with_parameters, T, copyable_test_types) {
    BOOST_TEST_MESSAGE("running future with minimal copyable type and passed parameter " << typeid(T).name());

    test_setup setup;
    {
        auto sut = async(custom_scheduler<0>(), [](auto x)->T { return x + T(0); }, T(42));
        BOOST_REQUIRE(sut.valid() == true);
        BOOST_REQUIRE(sut.error().is_initialized() == false);

        while (!sut.get_try()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        BOOST_WARN_EQUAL(T(42) + T(0), *sut.get_try());
    }
    BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
}

BOOST_AUTO_TEST_CASE(future_constructed_minimal_fn_moveonly) {
    BOOST_TEST_MESSAGE("running future with move only type");
    
    test_setup setup;
    {
        auto sut = async(custom_scheduler<0>(), []()->v1::move_only {
            return v1::move_only{ 42 };
        });
        BOOST_REQUIRE(sut.valid() == true);
        BOOST_REQUIRE(sut.error().is_initialized() == false);
        
        while (!sut.get_try()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        
        BOOST_REQUIRE_EQUAL(42, sut.get_try().get().member());
    }
    BOOST_REQUIRE_EQUAL(1, custom_scheduler<0>::usage_counter());
}

BOOST_AUTO_TEST_CASE(future_equality_tests)
{
    BOOST_TEST_MESSAGE("running future equality tests");
    {
        future<int> a;
        future<int> b;
        BOOST_REQUIRE(a == b);
        BOOST_REQUIRE(!(a != b));
    }

    {
        future<void> a;
        future<void> b;
        BOOST_REQUIRE(a == b);
        BOOST_REQUIRE(!(a != b));
    }

    {
        future<move_only> a;
        future<move_only> b;
        BOOST_REQUIRE(a == b);
        BOOST_REQUIRE(!(a != b));
    }

    {
        future<int> a = async(default_executor, [] { return 42; });
        auto b = a;
        BOOST_REQUIRE(a == b);
    }
    {
        future<void> a = async(default_executor, [] {});
        auto b = a;
        BOOST_REQUIRE(a == b);
    }

    {
        future<int> a = async(default_executor, [] { return 42; });
        future<int> b = async(default_executor, [] { return 42; });
        BOOST_REQUIRE(a != b);
    }
    {
        future<void> a = async(default_executor, [] {});
        future<void> b = async(default_executor, [] {});
        BOOST_REQUIRE(a != b);
    }
    {
        future<move_only> a = async(default_executor, [] { return move_only(42); });
        future<move_only> b;
        BOOST_REQUIRE(a != b);
    }
}

BOOST_AUTO_TEST_CASE(future_swap_tests)
{
    {
        auto a = package<int(int)>(immediate_executor, [](int a) { return a + 2; });
        auto b = package<int(int)>(immediate_executor, [](int a) { return a + 4; });

        std::swap(a, b);

        a.first(1);
        b.first(2);

        BOOST_REQUIRE_EQUAL(5, a.second.get_try().value());
        BOOST_REQUIRE_EQUAL(4, b.second.get_try().value());
    }
    {
        int x(0), y(0);
        auto a = package<void(int)>(immediate_executor, [&x](int a) { x = a + 2; });
        auto b = package<void(int)>(immediate_executor, [&y](int a) { y = a + 4; });

        std::swap(a, b);

        a.first(1);
        b.first(2);

        BOOST_REQUIRE_EQUAL(5, y);
        BOOST_REQUIRE_EQUAL(4, x);
    }
    {
        auto a = package<move_only(int)>(immediate_executor, [](int a) { return move_only(a + 2); });
        auto b = package<move_only(int)>(immediate_executor, [](int a) { return move_only(a + 4); });

        std::swap(a, b);

        a.first(1);
        b.first(2);

        BOOST_REQUIRE_EQUAL(5, a.second.get_try().value().member());
        BOOST_REQUIRE_EQUAL(4, b.second.get_try().value().member());
    }
}

BOOST_FIXTURE_TEST_SUITE(future_then_void, test_fixture<int>)

    BOOST_AUTO_TEST_CASE(future_get_try_refref) {
        BOOST_TEST_MESSAGE("future get_try()&& accessor test");

        auto sut = async(default_executor, [] { return 42; })
            .then([](int val) {
                throw test_exception("failure");
                return 0;
            })
            .recover([](auto &&f) {
                try {
                    std::forward<decltype(f)>(f).get_try();
                    return 0;
                }
                catch (const test_exception &) {
                    return 42;
                }
            });

        wait_until_future_completed(sut);
        BOOST_REQUIRE_EQUAL(42, sut.get_try().value());
    }
BOOST_AUTO_TEST_SUITE_END()



BOOST_AUTO_TEST_CASE(future_blocking_get_copyable_value)
{
    BOOST_TEST_MESSAGE("future blocking_get with copyable value");
    auto answer = [] { return 42; };

    stlab::future<int> f = 
      stlab::async(stlab::default_executor, answer);


    BOOST_REQUIRE_EQUAL(42, stlab::blocking_get(f));
}


BOOST_AUTO_TEST_CASE(future_blocking_get_moveonly_value)
{
    BOOST_TEST_MESSAGE("future blocking_get with moveonly value");
    auto answer = [] { return stlab::move_only(42); };

    stlab::future<stlab::move_only> f =
        stlab::async(stlab::default_executor, answer);


    BOOST_REQUIRE_EQUAL(42, stlab::blocking_get(std::move(f)).member());
}


BOOST_AUTO_TEST_CASE(future_blocking_get_void)
{
    BOOST_TEST_MESSAGE("future blocking_get with void");
    int v = 0;
    auto answer = [&] { v = 42; };

    stlab::future<void> f =
        stlab::async(stlab::default_executor, answer);


    stlab::blocking_get(f);
    BOOST_REQUIRE_EQUAL(42, v);
}

BOOST_AUTO_TEST_CASE(future_blocking_get_copyable_value_error_case)
{
    BOOST_TEST_MESSAGE("future blocking_get with copyable value error case");
    auto answer = [] { throw test_exception("failure"); return 42; };

    stlab::future<int> f =
        stlab::async(stlab::default_executor, answer);


    BOOST_REQUIRE_EXCEPTION(stlab::blocking_get(f), test_exception,
                            ([](const auto& e) { return std::string(e.what()) == std::string("failure"); }));
}


BOOST_AUTO_TEST_CASE(future_blocking_get_moveonly_value_error_case)
{
    BOOST_TEST_MESSAGE("future blocking_get with moveonly value");
    auto answer = [] { throw test_exception("failure"); return stlab::move_only(42); };

    stlab::future<stlab::move_only> f =
        stlab::async(stlab::default_executor, answer);


    BOOST_REQUIRE_EXCEPTION(stlab::blocking_get(std::move(f)), test_exception,
                            ([](const auto& e) { return std::string(e.what()) == std::string("failure"); }));
}


BOOST_AUTO_TEST_CASE(future_blocking_get_void_error_case)
{
    BOOST_TEST_MESSAGE("future blocking_get with void error case");
    
    auto answer = [] { throw test_exception("failure"); };

    stlab::future<void> f =
        stlab::async(stlab::default_executor, answer);


    BOOST_REQUIRE_EXCEPTION(stlab::blocking_get(f), test_exception,
                            ([](const auto& e) { return std::string(e.what()) == std::string("failure"); }));
}

