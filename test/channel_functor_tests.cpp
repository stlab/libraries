/*
Copyright 2016 Felix Petriconi
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <stlab/channel.hpp>

BOOST_AUTO_TEST_CASE(int_channel_void_functor_one_value) {
    BOOST_TEST_MESSAGE("int channel void functor one value");

    stlab::sender<int> send;
    stlab::receiver<int> receive;
    std::tie(send, receive) = stlab::channel<int>(stlab::default_scheduler());
    std::atomic_int result{ 0 };

    auto check = receive | [&_result = result](int x) { _result += x; };

    receive.set_ready();
    send(1);

    while (result < 1) {
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }

    BOOST_REQUIRE_EQUAL(1, result);
}

BOOST_AUTO_TEST_CASE(int_channel_void_functor_two_values) {
    BOOST_TEST_MESSAGE("int channel void functor two values");

    stlab::sender<int> send;
    stlab::receiver<int> receive;
    std::tie(send, receive) = stlab::channel<int>(stlab::default_scheduler());
    std::atomic_int result{ 0 };

    auto check = receive | [&_result = result](int x) { _result += x; };

    receive.set_ready();
    send(1);
    send(1);

    while (result < 2) {
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }

    BOOST_REQUIRE_EQUAL(2, result);
}

BOOST_AUTO_TEST_CASE(int_channel_void_functor_many_values) {
    BOOST_TEST_MESSAGE("int channel void functor many values");

    stlab::sender<int> send;
    stlab::receiver<int> receive;
    std::tie(send, receive) = stlab::channel<int>(stlab::default_scheduler());
    std::atomic_int result{ 0 };

    auto check = receive | [&_result = result](int x) { _result += x; };

    receive.set_ready();
    for (int i = 0; i < 10; ++i) send(1);

    while (result < 10) {
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }

    BOOST_REQUIRE_EQUAL(10, result);
}


#ifdef STLAB_CHANNEL_MOVE_ONLY_SUPPORT

BOOST_AUTO_TEST_CASE(move_only_int_channel_void_functor) {
    BOOST_TEST_MESSAGE("move only int channel void functor");

    stlab::sender<std::unique_ptr<int>> send;
    stlab::receiver<std::unique_ptr<int>> receive;
    std::tie(send, receive) = stlab::channel<std::unique_ptr<int>>(stlab::default_scheduler());
    std::atomic_int result{ 0 };

    auto check = receive | [&_result = result](std::unique_ptr<int> x) { _result += *x; };

    receive.set_ready();
    for (int i = 0; i < 10; ++i) {
        auto arg = std::make_unique<int>();
        *arg = 1;
        send(std::move(arg));
    }

    while (result < 10) {
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }

    BOOST_REQUIRE_EQUAL(10, result);
}

#endif

BOOST_AUTO_TEST_CASE(int_channel_split_void_functor) {
    BOOST_TEST_MESSAGE("int channel void functor");

    stlab::sender<int> send;
    stlab::receiver<int> receive;
    std::tie(send, receive) = stlab::channel<int>(stlab::default_scheduler());
    std::atomic_int result1{ 0 };
    std::atomic_int result2{ 0 };

    auto check1 = receive | [&_result = result1](int x) { _result += x; };
    auto check2 = receive | [&_result = result2](int x) { _result += x; };

    receive.set_ready();
    for (int i = 0; i < 10; ++i) send(1);

    while (result1 < 10 && result2 < 10) {
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }

    BOOST_REQUIRE_EQUAL(10, result1);
    BOOST_REQUIRE_EQUAL(10, result2);
}


BOOST_AUTO_TEST_CASE(int_channel_int_functor) {
    BOOST_TEST_MESSAGE("int channel int functor");

    stlab::sender<int> send;
    stlab::receiver<int> receive;
    std::tie(send, receive) = stlab::channel<int>(stlab::default_scheduler());
    std::atomic_int result{ 0 };

    auto check = receive | [](int x) { return x + x; } | [&_result = result](int x) { _result += x; };

    receive.set_ready();
    for (int i = 0; i < 10; ++i) send(1);

    while (result < 20) {
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }

    BOOST_REQUIRE_EQUAL(20, result);
}

#ifdef STLAB_CHANNEL_MOVE_ONLY_SUPPORT

BOOST_AUTO_TEST_CASE(move_only_int_channel_int_functor) {
    BOOST_TEST_MESSAGE("move only int channel int functor");

    stlab::sender<std::unique_ptr<int>> send;
    stlab::receiver<std::unique_ptr<int>> receive;
    std::tie(send, receive) = stlab::channel<std::unique_ptr<int>>(stlab::default_scheduler());
    std::atomic_int result{ 0 };

    auto check = receive | [](std::unique<int> x) { *x += *x; return std::move(x); } | [&_result = result](std::unique_ptr<int> x) { _result += *x; };

    receive.set_ready();
    for (int i = 0; i < 10; ++i) {
        auto arg = std::make_unique<int>();
        *arg = 1;
        send(std::move(arg));
    }

    while (result < 20) {
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }

    BOOST_REQUIRE_EQUAL(20, result);
}

#endif


BOOST_AUTO_TEST_CASE(int_channel_split_int_functor) {
    BOOST_TEST_MESSAGE("int channel int functor");

    stlab::sender<int> send;
    stlab::receiver<int> receive;
    std::tie(send, receive) = stlab::channel<int>(stlab::default_scheduler());
    std::atomic_int result1{ 0 };
    std::atomic_int result2{ 0 };

    auto check1 = receive | [](int x) { return x + x; } | [&_result = result1](int x) { _result += x; };
    auto check2 = receive | [](int x) { return x + x; } | [&_result = result2](int x) { _result += x; };

    receive.set_ready();
    for (int i = 0; i < 10; ++i) send(1);

    while (result1 < 20 && result2 < 20) {
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }

    BOOST_REQUIRE_EQUAL(20, result1);
    BOOST_REQUIRE_EQUAL(20, result2);
}


BOOST_AUTO_TEST_CASE(int_channel_split_int_functor_async) {
    BOOST_TEST_MESSAGE("int channel int functor asynchronous");

    stlab::sender<int> send;
    stlab::receiver<int> receive;
    std::tie(send, receive) = stlab::channel<int>(stlab::default_scheduler());
    std::vector<stlab::future<void>> inputs;
    for (auto i = 0; i < 10; ++i)
    {
        inputs.emplace_back(stlab::async(stlab::default_scheduler(),
            [&_send = send, i]() { _send(i); }));
    }

    std::atomic_int result1{ 0 };
    std::atomic_int result2{ 0 };

    auto check1 = receive | [](int x) { return x + x; } | [&_result = result1](int x) { _result += x; };
    auto check2 = receive | [](int x) { return x + x; } | [&_result = result2](int x) { _result += x; };

    receive.set_ready();

    while (result1 < 90 && result2 < 90) {
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }

    BOOST_REQUIRE_EQUAL(90, result1);
    BOOST_REQUIRE_EQUAL(90, result2);
}

