/*
    Copyright 2013 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
/**************************************************************************************************/

// #include <adobe/config.hpp>

#include <functional>
#include <iostream>
#include <utility>

#include <boost/test/unit_test.hpp>
// #include <adobe/test/check_regular.hpp>
// #include <adobe/test/check_less_than_comparable.hpp>
// #include <adobe/test/check_null.hpp>

#include <stlab/copy_on_write.hpp>
//#include <adobe/memory.hpp>

/**************************************************************************************************/

namespace {

/**************************************************************************************************/

template <typename R, typename T>
R make_value(const T& x) {
    return R(x);
}

/**************************************************************************************************/

template <>
std::string make_value(const long& x) {
    std::stringstream s;
    s << x;
    return std::string(s.str());
}

/**************************************************************************************************/

template <typename CowType>
void test_copy_on_write() {
    typename CowType::value_type (*mv)(const long&) =
        &make_value<typename CowType::value_type, long>;

    // Test default constructor
    { CowType value_0; }

    // Test basic concept requirements
    {
        CowType value_1(mv(1)); // allocation
        CowType value_2(mv(2)); // allocation
        CowType value_3(mv(3)); // allocation

        // regular
        // adobe::check_regular(value_1);

        // operator<
        // adobe::check_less_than_comparable(value_1, value_2, value_3, std::less<CowType>());

        // operator>
        // adobe::check_less_than_comparable(value_3, value_2, value_1, std::greater<CowType>());

        CowType value_test(mv(1)); // allocation

        BOOST_CHECK_MESSAGE(value_1 == value_test, "equality of non-identical values");
        BOOST_CHECK_MESSAGE(value_2 != value_test, "equality of non-identical values");

        BOOST_CHECK(value_test.unique());

        value_test = value_2; // deallocation

        BOOST_CHECK(!value_test.unique());
        BOOST_CHECK(value_test.identity(value_2));
    }

    // Test basic move semantics
    {
        CowType value_1(mv(42)); // allocation
        CowType value_2(mv(21)); // allocation
        CowType value_move(std::move(value_1));

        // BOOST_CHECK_MESSAGE(value_move != value_1, "move failure");

        value_move = std::move(value_2); // deallocation

        // BOOST_CHECK_MESSAGE(value_move != value_2, "move failure");
        // BOOST_CHECK_MESSAGE(value_1 == value_2, "move failure"); // both should be object_m == 0
    }

    // Test copy-assignment using null object_m
    {
        CowType foo(mv(1)); // allocation
        CowType bar(std::move(foo));

        foo = mv(2); // allocation
    }

    // Test copy-assignment using non-null object_m
    {
        CowType foo(mv(5)); // allocation
        CowType bar(foo);

        BOOST_CHECK(bar.identity(foo));

        bar = mv(6); // allocation

        BOOST_CHECK(bar.unique() && foo.unique());
    }

    // Test move-assignment using null object_m
    {
        CowType foo(mv(1)); // allocation
        CowType bar(std::move(foo));
        typename CowType::value_type value(mv(2));

        foo = std::move(value); // allocation
    }

    // Test move-assignment using unique instance
    {
        CowType foo(mv(1)); // allocation
        typename CowType::value_type value(mv(2));

        foo = std::move(value);
    }

    // Test move-assignment using new allocation
    {
        CowType foo(mv(1)); // allocation
        CowType bar(foo);
        typename CowType::value_type value(mv(2));

        foo = std::move(value); // allocation
    }

    // Test write() using unique instance
    {
        CowType foo(mv(1)); // allocation

        foo.write() = typename CowType::value_type(mv(2));
    }

    // Test write() using new allocation
    {
        CowType foo(mv(1)); // allocation
        CowType bar(foo);

        foo.write() = typename CowType::value_type(mv(2)); // allocation
    }

    // Test read()
    {
        CowType foo(mv(1)); // allocation

        BOOST_CHECK_MESSAGE(foo.read() == typename CowType::value_type(mv(1)), "read error");
        BOOST_CHECK_MESSAGE(static_cast<typename CowType::value_type>(foo) ==
                                typename CowType::value_type(mv(1)),
                            "read error");
        BOOST_CHECK_MESSAGE(*foo == typename CowType::value_type(mv(1)), "read error");
        BOOST_CHECK_MESSAGE(*(foo.operator->()) == typename CowType::value_type(mv(1)),
                            "read error");
    }

    // Test swap
    {
        CowType foo(mv(1)); // allocation
        CowType bar(mv(2)); // allocation

        swap(foo, bar);

        BOOST_CHECK_MESSAGE(foo.read() == typename CowType::value_type(mv(2)), "swap error");
        BOOST_CHECK_MESSAGE(bar.read() == typename CowType::value_type(mv(1)), "swap error");
    }
}

/**************************************************************************************************/

} // namespace

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(copy_on_write_interface) {
    using namespace stlab;

    {
        // default construction
        copy_on_write<int> a;
        copy_on_write<int> b;

        BOOST_CHECK_MESSAGE(a.identity(b), "default construction error");
        BOOST_CHECK_MESSAGE(a == 0, "default construction error");
    }

    {
        // emplace construction
        copy_on_write<int> a(10);
        copy_on_write<std::pair<int, int>> b(10, 20);
        copy_on_write<std::tuple<int, int, int>> c(10, 20, 30);

        BOOST_CHECK_MESSAGE(a == 10, "value construction error");
        BOOST_CHECK_MESSAGE((b == std::make_pair(10, 20)), "value construction error");
        BOOST_CHECK_MESSAGE((c == std::make_tuple(10, 20, 30)), "value construction error");
    }

    {
        // copy construction
        copy_on_write<int> a = 3;
        copy_on_write<int> b = a;
        BOOST_CHECK_MESSAGE(a.identity(b), "copy construction error");
        BOOST_CHECK_MESSAGE(b == 3, "copy construction error");
    }

    {
        // move construction
        copy_on_write<int> a = 3;
        copy_on_write<int> b = std::move(a);
        a = 0;
        BOOST_CHECK_MESSAGE(!a.identity(b), "move construction error");
        BOOST_CHECK_MESSAGE(b == 3, "move construction error");
        BOOST_CHECK_MESSAGE(a == 0, "move construction error");
    }

    {
        // copy assignment
        copy_on_write<int> a = 3;
        copy_on_write<int> b;
        b = a;
        BOOST_CHECK_MESSAGE(a.identity(b), "copy assignment error");
        BOOST_CHECK_MESSAGE(b == 3, "copy assignment error");
    }

    {
        // move assignment
        copy_on_write<int> a = 3;
        copy_on_write<int> b;
        b = std::move(a);
        a = 0;
        BOOST_CHECK_MESSAGE(!a.identity(b), "move assignment error");
        BOOST_CHECK_MESSAGE(b == 3, "move assignment error");
        BOOST_CHECK_MESSAGE(a == 0, "move assignment error");
    }

    {
        // value assignment
        copy_on_write<int> a;
        a = 3;
        BOOST_CHECK_MESSAGE(a == 3, "value assignment error");
    }

    {
        // write
        copy_on_write<std::pair<int, int>> a(1, 2);
        copy_on_write<std::pair<int, int>> b = a;
        BOOST_CHECK(a.identity(b));
        a.write().first = 3;
        BOOST_CHECK(!a.identity(b));
        BOOST_CHECK(a == std::make_pair(3, 2));
        BOOST_CHECK(b == std::make_pair(1, 2));
    }

    {
        // read
        copy_on_write<std::pair<int, int>> a(1, 2);
        BOOST_CHECK(a.read().first == 1 && a.read().second == 2);
    }

    {
        // implicit conversion
        copy_on_write<std::pair<int, int>> a(1, 2);
        std::pair<int, int> b = a;
        BOOST_CHECK(b == std::make_pair(1, 2));
    }

    {
        // operator * and ->
        copy_on_write<std::pair<int, int>> a(1, 2);
        BOOST_CHECK(a->first == 1 && a->second == 2);
        BOOST_CHECK((*a).first == 1 && (*a).second == 2);
    }

    {
        // unique
        copy_on_write<std::pair<int, int>> a(1, 2);
        BOOST_CHECK(a.unique());
        {
            auto b = a;
            BOOST_CHECK(!a.unique());
        }
        BOOST_CHECK(a.unique());
    }

    // identity (tested above)

    // swap
    {
        copy_on_write<int> a(1);
        copy_on_write<int> b(2);
        swap(a, b);
        BOOST_CHECK((a == 2) && (b == 1));
    }

    // comparisons
    {
        copy_on_write<int> a(1);
        copy_on_write<int> b(1);
        copy_on_write<int> c(2);

        BOOST_CHECK((a == b) && (a != c) && !(a == c) && !(a != b));
        BOOST_CHECK((a == 1) && (a != 2) && !(a == 2) && !(a != 1));
        BOOST_CHECK((1 == b) && (1 != c) && !(1 == c) && !(1 != b));

        BOOST_CHECK(!(a < b) && (a < c));
        BOOST_CHECK(!(a < 1) && (a < 2));
        BOOST_CHECK(!(1 < 1) && (1 < 2));

        BOOST_CHECK(!(a > b) && (c > a));
        BOOST_CHECK(!(a > 1) && (c > 1));
        BOOST_CHECK(!(1 > b) && (2 > a));

        BOOST_CHECK((a <= b) && !(c <= a));
        BOOST_CHECK((a <= 1) && !(c <= 1));
        BOOST_CHECK((1 <= b) && !(2 <= a));

        BOOST_CHECK((a >= b) && !(a >= c));
        BOOST_CHECK((a >= 1) && !(a >= 2));
        BOOST_CHECK((1 >= b) && !(1 >= c));
    }
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(copy_on_write_test) {
    // test nonmovable type with capture_allocator
    test_copy_on_write<stlab::copy_on_write<int>>();

    // test movable type with capture_allocator
    test_copy_on_write<stlab::copy_on_write<std::string>>();
}

/**************************************************************************************************/
