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
    return stlab::await(a([](auto x) { return x; }));
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(actor_construct_with_arguments) {
    stlab::actor<int> a(stlab::default_executor, "actor_int", 42);
    stlab::future<void> f = a([](auto i) { BOOST_REQUIRE(i == 42); });

    stlab::await(f);

    BOOST_REQUIRE(get_actor_value(a) == 42);
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(actor_construct_void) {
    stlab::actor<void> a(stlab::default_executor, "actor_void");
    bool sent{false};
    stlab::future<void> f = a([&]() { sent = true; });

    stlab::await(f);

    BOOST_REQUIRE(sent);
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(actor_regularity) {
    stlab::actor<int> empty_ctor;

    stlab::actor<int> default_ctor(stlab::default_executor, "foo"); // default construction
    default_ctor(increment).detach();
    BOOST_REQUIRE(get_actor_value(default_ctor) == 1);

    stlab::actor<int> copy_ctor(default_ctor); // copy construction
    copy_ctor(increment).detach();
    BOOST_REQUIRE(get_actor_value(copy_ctor) == 2);

    stlab::actor<int> move_ctor(std::move(default_ctor)); // move construction
    move_ctor(increment).detach();
    BOOST_REQUIRE(get_actor_value(move_ctor) == 3);

    stlab::actor<int> copy_assign = copy_ctor; // copy assignment
    copy_assign(increment).detach();
    BOOST_REQUIRE(get_actor_value(copy_assign) == 4);

    stlab::actor<int> move_assign = std::move(move_ctor); // move assignment
    move_assign(increment).detach();
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
    stlab::await(default_ctor([&] { ++count; }));
    BOOST_REQUIRE(count == 1);

    stlab::actor<void> copy_ctor(default_ctor); // copy construction
    stlab::await(copy_ctor([&] { ++count; }));
    BOOST_REQUIRE(count == 2);

    stlab::actor<void> move_ctor(std::move(default_ctor)); // move construction
    stlab::await(move_ctor([&] { ++count; }));
    BOOST_REQUIRE(count == 3);

    stlab::actor<void> copy_assign = move_ctor; // copy assignment
    stlab::await(copy_assign([&] { ++count; }));
    BOOST_REQUIRE(count == 4);

    stlab::actor<void> move_assign = std::move(move_ctor); // move assignment
    stlab::await(move_assign([&] { ++count; }));
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
        stlab::future<void> f = a(increment);

        stlab::await(f);

        BOOST_REQUIRE(f.get_try());
    }

    {
        stlab::actor<void> a(stlab::default_executor, "send_getting_void");
        std::atomic_bool sent{false};
        stlab::future<void> f = a([&] { sent = true; });

        stlab::await(f);

        BOOST_REQUIRE(sent);
    }
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(actor_send_to_value) {
    {
        stlab::actor<int> a(stlab::default_executor, "send_getting_value", 42);
        stlab::future<int> f = a([](auto x) { return x; });
        int result = stlab::await(f);

        BOOST_REQUIRE(result == 42);
    }

    {
        stlab::actor<void> a(stlab::default_executor, "send_getting_value");
        stlab::future<int> f = a([]() { return 42; });
        int result = stlab::await(f);

        BOOST_REQUIRE(result == 42);
    }
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(actor_then_from_void) {
    {
        stlab::actor<int> a(stlab::default_executor, "send_then_from_void");
        stlab::future<int> f =
            a([](int& x) { x += 42; }).then(a.executor(), a.entask([](int x) { return x; }));

        int result = stlab::await(f);

        BOOST_REQUIRE(result == 42);
    }

    {
        stlab::actor<void> a(stlab::default_executor, "send_then_from_void");
        stlab::future<int> f = a([]() { return 42; })
                                   .then(a.executor(), a.entask([](auto x) { return 4200 + x; }))
                                   .then(a.executor(), a.entask([](auto x) { return x + 420000; }));

        int result = stlab::await(f);

        BOOST_REQUIRE(result == 424242);
    }
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(actor_then_from_value) {
    stlab::actor<int> a(stlab::default_executor, "send_then_from_type", 42);
    stlab::future<int> f =
        a([](auto x) { return x; }).then(a.executor(), a.entask([](auto x, auto y) {
            BOOST_REQUIRE(x == 42);
            BOOST_REQUIRE(y == 42);
            return x + y;
        }));

    int result = stlab::await(f);

    BOOST_REQUIRE(result == 84);
}

/**************************************************************************************************/

BOOST_AUTO_TEST_CASE(actor_this_actor) {
    stlab::actor<void> a(stlab::default_executor, "this_actor");

    try {
        stlab::this_actor::get<void>();
        BOOST_REQUIRE(false); // "Expected a throw - not run in an actor";
    } catch (...) { }

    BOOST_REQUIRE(stlab::this_actor::get_id() == stlab::actor_id{0});

    auto f = a([_a = a](){
        auto this_instance = stlab::this_actor::get<void>();
        BOOST_REQUIRE(this_instance == _a);

        auto this_actor_id = stlab::this_actor::get_id();
        BOOST_REQUIRE(this_actor_id == _a.get_id());
        BOOST_REQUIRE(this_actor_id != stlab::actor_id{0});
    });

    stlab::await(f);
}

/**************************************************************************************************/
