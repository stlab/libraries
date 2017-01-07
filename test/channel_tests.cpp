/*
Copyright 2016 Felix Petriconi
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <stlab/channel.hpp>

#include "channel_test_helper.hpp"

BOOST_AUTO_TEST_CASE(int_sender) {
    BOOST_TEST_MESSAGE("int sender");

    stlab::sender<int> send;

    BOOST_REQUIRE_NO_THROW(send(42));
}


BOOST_AUTO_TEST_CASE(int_receiver) {
    BOOST_TEST_MESSAGE("int receiver");

    stlab::receiver<int> receive;

    BOOST_REQUIRE_EQUAL(false, receive.ready());

    receive.set_ready();
    BOOST_REQUIRE_EQUAL(true, receive.ready());
}

using channel_test_fixture_int_1 = channel_test_fixture<int, 1>;

BOOST_FIXTURE_TEST_SUITE(int_channel, channel_test_fixture_int_1)
    BOOST_AUTO_TEST_CASE(int_channel) {
        BOOST_TEST_MESSAGE("int channel");

        BOOST_REQUIRE_EQUAL(false, _receive[0].ready());

        _receive[0].set_ready();
        BOOST_REQUIRE_EQUAL(true, _receive[0].ready());
    }


    BOOST_AUTO_TEST_CASE(int_channel_move_ctor_sender) {
        BOOST_TEST_MESSAGE("int channel move ctor sender");

        std::atomic_int result{0};

        auto check = _receive[0] | [&](int x) { result = x; };
        auto sut = std::move(_send[0]);

        _receive[0].set_ready();
        sut(42);

        wait_until_done([&] { return result == 42; });

        BOOST_REQUIRE_EQUAL(42, result);
    }

    BOOST_AUTO_TEST_CASE(int_channel_move_assignment_sender) {
        BOOST_TEST_MESSAGE("int channel move assignment sender");

        stlab::sender<int> sut;
        std::atomic_int result{ 0 };

        auto check = _receive[0] | [&](int x) { result = x; };
        sut = std::move(_send[0]);

        _receive[0].set_ready();
        sut(42);

        wait_until_done([&] { return result == 42; });

        BOOST_REQUIRE_EQUAL(42, result);
    }

    BOOST_AUTO_TEST_CASE(int_channel_copy_ctor_sender) {
        BOOST_TEST_MESSAGE("int channel copy ctor sender");

        std::atomic_int result{ 0 };

        auto check = _receive[0] | [&](int x) { result = x; };
        auto sut(_send[0]);

        _receive[0].set_ready();
        sut(42);

        wait_until_done([&] { return result == 42; });

        BOOST_REQUIRE_EQUAL(42, result);

        result = 0;

        _send[0](4711);

        wait_until_done([&] { return result == 4711; });

        BOOST_REQUIRE_EQUAL(4711, result);
    }

    BOOST_AUTO_TEST_CASE(int_channel_copy_assignment_sender) {
        BOOST_TEST_MESSAGE("int channel copy assignment sender");

        stlab::sender<int> sut;
        std::atomic_int result{ 0 };

        auto check = _receive[0] | [&](int x) { result = x; };
        sut = _send[0];

        _receive[0].set_ready();
        sut(42);

        wait_until_done([&] { return result == 42; });

        BOOST_REQUIRE_EQUAL(42, result);
    
        result = 0;
        
        _send[0](4711);

        wait_until_done([&] { return result == 4711; });

        BOOST_REQUIRE_EQUAL(4711, result);
    }

    BOOST_AUTO_TEST_CASE(int_channel_move_ctor_receiver) {
        BOOST_TEST_MESSAGE("int channel move ctor receiver");
        
        std::atomic_int result{ 0 };
    
        auto sut = std::move(_receive[0]);

        auto check = sut | [&](int x) { result = x; };
        sut.set_ready();
        _send[0](42);

        wait_until_done([&] { return result == 42; });

        BOOST_REQUIRE_EQUAL(42, result);
    }

    BOOST_AUTO_TEST_CASE(int_channel_move_assignment_receiver) {
        BOOST_TEST_MESSAGE("int channel move assignment receiver");

        stlab::receiver<int> sut;
        std::atomic_int result{ 0 };

        sut = std::move(_receive[0]);
        auto check = sut | [&](int x) { result = x; };

        sut.set_ready();
        _send[0](42);

        wait_until_done([&] { return result == 42; });

        BOOST_REQUIRE_EQUAL(42, result);
    }

    BOOST_AUTO_TEST_CASE(int_channel_copy_ctor_receiver) {
        BOOST_TEST_MESSAGE("int channel copy ctor receiver");

        std::atomic_int result{ 0 };

        auto sut(_receive[0]);

        auto check = sut | [&](int x) { result = x; };
    
        _receive[0].set_ready();
        sut.set_ready();
        _send[0](42);

        wait_until_done([&]() { return result == 42; });

        BOOST_REQUIRE_EQUAL(42, result);
    }

    BOOST_AUTO_TEST_CASE(int_channel_copy_assignment_receiver) {
        BOOST_TEST_MESSAGE("int channel copy assignment receiver");

        stlab::receiver<int> sut;
        std::atomic_int result{ 0 };

        sut = _receive[0];

        auto check = sut | [&](int x) { result = x; };

        _receive[0].set_ready();
        sut.set_ready();

        _send[0](42);

        wait_until_done([&] { return result == 42; });

        BOOST_REQUIRE_EQUAL(42, result);
    }
BOOST_AUTO_TEST_SUITE_END()




BOOST_AUTO_TEST_CASE(int_channel_one_value_different_buffer_sizes) {
    BOOST_TEST_MESSAGE("int channel one value different buffer sizes");

    for (auto bs : { 0,1,2,10 }) {
        stlab::sender<int> send;
        stlab::receiver<int> receive;
        std::tie(send, receive) = stlab::channel<int>(stlab::default_executor());
        std::atomic_int result{ 0 };

        auto check = receive | stlab::buffer_size(bs) & [&](int x) { result += x; };

        receive.set_ready();
        send(1);

        while (result < 1) {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }

        BOOST_REQUIRE_EQUAL(1, result);
    }
}

BOOST_AUTO_TEST_CASE(int_channel_two_values_different_buffer_sizes) {
    BOOST_TEST_MESSAGE("int channel two values different buffer sizes");

    for (auto bs : { 0,1,2,10 }) {
        stlab::sender<int> send;
        stlab::receiver<int> receive;
        std::tie(send, receive) = stlab::channel<int>(stlab::default_executor());
        std::atomic_int result{ 0 };

        auto check = receive | stlab::buffer_size(bs) & [&](int x) { result += x; };

        receive.set_ready();
        send(1);
        send(1);

        while (result < 2) {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }

        BOOST_REQUIRE_EQUAL(2, result);
    }
}

BOOST_AUTO_TEST_CASE(int_channel_many_values_different_buffer_sizes) {
    BOOST_TEST_MESSAGE("int channel many values different buffer sizes");

    for (auto bs : { 0,1,2,10 }) {
        stlab::sender<int> send;
        stlab::receiver<int> receive;
        std::tie(send, receive) = stlab::channel<int>(stlab::default_executor());
        std::atomic_int result{ 0 };

        auto check = receive | stlab::buffer_size(bs) & [&](int x) { result += x; };

        receive.set_ready();
        for (auto i = 0; i < 10;++i) send(1);

        while (result < 10) {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }

        BOOST_REQUIRE_EQUAL(10, result);
    }
}


