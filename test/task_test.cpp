/*
    Copyright 2017 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
/**************************************************************************************************/

// stdc++
#include <iostream>

// boost
#include <boost/test/unit_test.hpp>

// stlab
#include <stlab/concurrency/task.hpp>
#include <stlab/test/model.hpp>

/**************************************************************************************************/

using namespace stlab;

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(task_nullary_tests) {
    {
        task<regular()> t([] { return regular(42); });
        BOOST_CHECK_EQUAL(t()._x, 42);
    }

    {
        task<regular()> x([] { return regular(42); });
        task<regular()> y = std::move(x);
        BOOST_CHECK_EQUAL(y()._x, 42);
    }

    {
        task<regular()> x([] { return regular(42); });
        task<regular()> y([] { return regular(99); });
        swap(x, y);
        BOOST_CHECK_EQUAL(x()._x, 99);
        BOOST_CHECK_EQUAL(y()._x, 42);
    }

    {
        task<move_only()> t([] { return move_only(42); });
        BOOST_CHECK_EQUAL(t().member(), 42);
    }

    {
        task<move_only()> x([] { return move_only(42); });
        task<move_only()> y = std::move(x);
        BOOST_CHECK_EQUAL(y().member(), 42);
    }
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(task_copyable_nullary_tests) {
    int value(42);

    {
        task<int()> t([_value = value] { return _value; });
        BOOST_CHECK_EQUAL(t(), value);
    }

    {
        task<int()> x([_value = value] { return _value; });
        task<int()> y = std::move(x);
        BOOST_CHECK_EQUAL(y(), value);
    }

    {
        task<regular()> x([_value = value] { return regular(_value); });
        task<regular()> y([_value = 99] { return regular(_value); });
        swap(x, y);
        BOOST_CHECK_EQUAL(x()._x, 99);
        BOOST_CHECK_EQUAL(y()._x, 42);
    }

    {
        task<move_only()> t([_value = value] { return move_only(_value); });
        BOOST_CHECK_EQUAL(t().member(), value);
    }

    {
        task<move_only()> x([_value = value] { return move_only(_value); });
        task<move_only()> y = std::move(x);
        BOOST_CHECK_EQUAL(y().member(), value);
    }
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(task_moveable_nullary_tests) {
    {
        move_only value(42);
        task<int()> t([_value = std::move(value)] { return _value.member(); });
        BOOST_CHECK_EQUAL(t(), 42);
    }

    {
        move_only value(42);
        task<int()> x([_value = std::move(value)] { return _value.member(); });
        task<int()> y = std::move(x);
        BOOST_CHECK_EQUAL(y(), 42);
    }

    {
        move_only value0(42);
        move_only value1(99);
        task<move_only()> x([_value = std::move(value0)]() mutable { return std::move(_value); });
        task<move_only()> y([_value = std::move(value1)]() mutable { return std::move(_value); });
        swap(x, y);
        BOOST_CHECK_EQUAL(x().member(), 99);
        BOOST_CHECK_EQUAL(y().member(), 42);
    }

    {
        move_only value(42);
        task<move_only()> t([_value = std::move(value)]() mutable { return std::move(_value); });
        BOOST_CHECK_EQUAL(t().member(), 42);
    }

    {
        move_only value(42);
        task<move_only()> x([_value = std::move(value)]() mutable { return std::move(_value); });
        task<move_only()> y = std::move(x);
        BOOST_CHECK_EQUAL(y().member(), 42);
    }
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(task_n_ary_tests) {
    {
        task<int(int)> t([](int x) { return x; });
        BOOST_CHECK_EQUAL(t(42), 42);
    }

    {
        task<int(int, float)> t([](int x, float y) { return x + static_cast<int>(y); });
        BOOST_CHECK_EQUAL(t(21, 21.), 42);
    }

    {
        task<int(int, float)> x([](int x, float y) { return x + static_cast<int>(y); });
        task<int(int, float)> y([](int x, float y) { return x * static_cast<int>(y); });
        swap(x, y);
        BOOST_CHECK_EQUAL(x(10, 10.), 100);
        BOOST_CHECK_EQUAL(y(10, 10.), 20);
    }

    {
        task<int(int, float, std::string)> t([](int x, float y, std::string z) {
            return x + static_cast<int>(y) + static_cast<int>(z.size());
        });
        BOOST_CHECK_EQUAL(t(20, 20., "00"), 42);
    }

    {
        task<int(move_only, int)> x([](move_only m, int i) { return m.member() + i; });
        BOOST_CHECK_EQUAL(42, x(move_only(40), 2));
    }
}

/**************************************************************************************************/

struct large_model {
    char buffer[512] = {42};
    auto operator()() const { return buffer[0]; }
};

BOOST_AUTO_TEST_CASE(task_type_tests) {
    {
        // empty model
        task<void()> t;
        BOOST_CHECK(!t);
        BOOST_CHECK_THROW(t(), std::bad_function_call);
        std::cout << t.target_type().name() << '\n';
        BOOST_CHECK(t.target<void>() == nullptr);
    }

    {
        // small model
        auto small_model = [] { return 42; };
        task<int()> t = small_model;
        BOOST_CHECK(t);
        BOOST_CHECK_EQUAL(t(), 42);
        std::cout << t.target_type().name() << '\n';
        BOOST_CHECK(t.target<decltype(small_model)>() != nullptr);

        // null assignment
        t = nullptr;
        BOOST_CHECK(!t);
    }

    {
        task<int()> t(nullptr);
        BOOST_CHECK(!t);
    }

    {
        // large model
        task<int()> t = large_model();
        BOOST_CHECK(t);
        BOOST_CHECK_EQUAL(t(), 42);
        std::cout << t.target_type().name() << '\n';
        BOOST_CHECK(t.target<decltype(large_model())>() != nullptr);
    }
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(task_equality_tests) {
    {
        task<void()> a;
        BOOST_CHECK(a == std::nullptr_t());
    }
    {
        task<void()> a;
        BOOST_CHECK(std::nullptr_t() == a);
    }

    {
        task<void()> a([] {});
        BOOST_CHECK(a != std::nullptr_t());
    }
    {
        task<void()> a([] {});
        BOOST_CHECK(std::nullptr_t() != a);
    }
}