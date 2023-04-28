/*
    Copyright 2023 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

// identity
#include <stlab/concurrency/actor.hpp>

// boost
#include <boost/test/unit_test.hpp>

// stlab
#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/utility.hpp>

/**************************************************************************************************/

void increment_by(int& i, int amount) { i += amount; }

void increment(int& i) { increment_by(i, 1); }

template <class T>
T get_actor_value(stlab::actor<T>& a) {
    return stlab::await(a.send([](auto& x) { return x; }));
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(actor_construct_with_arguments) {
    stlab::actor<int> a(stlab::default_executor, "actor_int", 42);
    stlab::future<void> f = a.send([](auto& i) { BOOST_REQUIRE(i == 42); });

    stlab::await(f);

    BOOST_REQUIRE(get_actor_value(a) == 42);
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(actor_construct_void) {
    stlab::actor<void> a(stlab::default_executor, "actor_void");
    std::atomic_bool sent{false};
    stlab::future<void> f = a.send([&]() { sent = true; });

    stlab::await(f);

    BOOST_REQUIRE(sent);
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(actor_regularity) {
    stlab::actor<int> empty_ctor;

    stlab::actor<int> default_ctor(stlab::default_executor, "foo"); // default construction
    default_ctor.send(increment).detach();
    BOOST_REQUIRE(get_actor_value(default_ctor) == 1);

    stlab::actor<int> copy_ctor(default_ctor); // copy construction
    copy_ctor.send(increment).detach();
    BOOST_REQUIRE(get_actor_value(copy_ctor) == 2);

    stlab::actor<int> move_ctor(std::move(default_ctor)); // move construction
    move_ctor.send(increment).detach();
    BOOST_REQUIRE(get_actor_value(move_ctor) == 3);

    stlab::actor<int> copy_assign = copy_ctor; // copy assignment
    copy_assign.send(increment).detach();
    BOOST_REQUIRE(get_actor_value(copy_assign) == 4);

    stlab::actor<int> move_assign = std::move(move_ctor); // move assignment
    move_assign.send(increment).detach();
    BOOST_REQUIRE(get_actor_value(move_assign) == 5);

    // equality comparable
    stlab::actor<int> a(stlab::default_executor, "a");
    stlab::actor<int> b(stlab::default_executor, "b");
    BOOST_REQUIRE(a != b);    // tests operator!=
    BOOST_REQUIRE(!(a == b)); // tests operator==

    BOOST_REQUIRE(a == a); // tests operator==
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(actor_regularity_void) {
    std::size_t count{0};
    stlab::actor<void> empty_ctor;

    stlab::actor<void> default_ctor(stlab::default_executor, "foo"); // default construction
    stlab::await(default_ctor.send([&] { ++count; }));
    BOOST_REQUIRE(count == 1);

    stlab::actor<void> copy_ctor(default_ctor); // copy construction
    stlab::await(copy_ctor.send([&] { ++count; }));
    BOOST_REQUIRE(count == 2);

    stlab::actor<void> move_ctor(std::move(default_ctor)); // move construction
    stlab::await(move_ctor.send([&] { ++count; }));
    BOOST_REQUIRE(count == 3);

    stlab::actor<void> copy_assign = move_ctor; // copy assignment
    stlab::await(copy_assign.send([&] { ++count; }));
    BOOST_REQUIRE(count == 4);

    stlab::actor<void> move_assign = std::move(move_ctor); // move assignment
    stlab::await(move_assign.send([&] { ++count; }));
    BOOST_REQUIRE(count == 5);

    // equality comparable
    stlab::actor<void> a(stlab::default_executor, "a");
    stlab::actor<void> b(stlab::default_executor, "b");
    BOOST_REQUIRE(a != b);    // tests operator!=
    BOOST_REQUIRE(!(a == b)); // tests operator==

    BOOST_REQUIRE(a == a); // tests operator==
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(actor_send_to_void) {
    {
        stlab::actor<int> a(stlab::default_executor, "send_getting_void");
        stlab::future<void> f = a.send(increment);

        stlab::await(f);

        BOOST_REQUIRE(f.get_try());
    }

    {
        stlab::actor<void> a(stlab::default_executor, "send_getting_void");
        std::atomic_bool sent{false};
        stlab::future<void> f = a.send([&] { sent = true; });

        stlab::await(f);

        BOOST_REQUIRE(sent);
    }
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(actor_send_to_value) {
    {
        stlab::actor<int> a(stlab::default_executor, "send_getting_value", 42);
        stlab::future<int> f = a.send([](auto& x) { return x; });
        int result = stlab::await(f);

        BOOST_REQUIRE(result == 42);
    }

    {
        stlab::actor<void> a(stlab::default_executor, "send_getting_value");
        stlab::future<int> f = a.send([]() { return 42; });
        int result = stlab::await(f);

        BOOST_REQUIRE(result == 42);
    }
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(actor_then_from_void) {
    {
        stlab::actor<int> a(stlab::default_executor, "send_then_from_void");
        stlab::future<void> f0 = a.send(increment_by, 42);
        stlab::future<int> f1 = a.then(stlab::future<void>(f0), [](auto& x) { return x; });
        stlab::future<void> f2 = a.then(std::move(f0), increment_by, 4200);
        stlab::future<int> f3 = a.then(std::move(f2), [](auto& x) { return x; });

        int result1 = stlab::await(f1);
        int result3 = stlab::await(f3);

        BOOST_REQUIRE(result1 == 42);
        BOOST_REQUIRE(result3 == 4242);
    }

    {
        stlab::actor<void> a(stlab::default_executor, "send_then_from_void");
        stlab::future<int> f0 = a.send([]() { return 42; });
        stlab::future<int> f1 = a.then(std::move(f0), [](auto x) { return 4200 + x; });
        stlab::future<int> f2 = a.then(
            std::move(f1), [](auto x, auto y) { return x + y; }, 420000);

        int result = stlab::await(f2);

        BOOST_REQUIRE(result == 424242);
    }
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(actor_then_from_value) {
    stlab::actor<int> a(stlab::default_executor, "send_then_from_type", 42);
    stlab::future<int> f0 = a.send([](auto& x) { return x; });
    stlab::future<int> f1 = a.then(stlab::future<int>(f0), [](auto& x, auto y) {
        BOOST_REQUIRE(x == 42);
        BOOST_REQUIRE(y == 42);
        return x + y;
    });
    stlab::future<int> f2 = a.then(
        std::move(f0),
        [](auto& x, auto y, auto z) {
            BOOST_REQUIRE(x == 42);
            BOOST_REQUIRE(y == 42);
            BOOST_REQUIRE(z == 100);
            return x + y + z;
        },
        100);

    int result1 = stlab::await(f1);
    int result2 = stlab::await(f2);

    BOOST_REQUIRE(result1 == 84);
    BOOST_REQUIRE(result2 == 184);
}

/**************************************************************************************************/
