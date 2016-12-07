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

using channel_test_fixture_int_1 = channel_test_fixture<int, 1>;

BOOST_FIXTURE_TEST_SUITE(int_channel_void_functor, channel_test_fixture_int_1)

    BOOST_AUTO_TEST_CASE(int_channel_void_functor_one_value) {
        BOOST_TEST_MESSAGE("int channel void functor one value");
        std::atomic_int result{ 0 };

        auto check = _receive[0] | [&_result = result](int x) { _result += x; };

        _receive[0].set_ready();
        _send[0](1);

        wait_until_done([&_result = result]() { return _result != 0; });

        BOOST_REQUIRE_EQUAL(1, result);
    }


    BOOST_AUTO_TEST_CASE(int_channel_void_functor_two_values) {
        BOOST_TEST_MESSAGE("int channel void functor two values");
        std::atomic_int result{ 0 };

        auto check = _receive[0] | [&_result = result](int x) { _result += x; };

        _receive[0].set_ready();
        _send[0](1);
        _send[0](2);

        wait_until_done([&_result = result]() { return _result == 3; });
        BOOST_REQUIRE_EQUAL(3, result);
    }

    BOOST_AUTO_TEST_CASE(int_channel_void_functor_many_values) {
        BOOST_TEST_MESSAGE("int channel void functor many values");

        std::atomic_int result{ 0 };

        auto check = _receive[0] | [&_result = result](int x) { _result += x;  };

        _receive[0].set_ready();
        for (int i = 0; i < 10; ++i)
            _send[0](1);

        wait_until_done([&_result = result]() { return _result == 10; });
        BOOST_REQUIRE_EQUAL(10, result);
    }


    BOOST_AUTO_TEST_CASE(int_channel_split_void_functor) {
        BOOST_TEST_MESSAGE("int channel void functor");

        std::atomic_int result1{ 0 };
        std::atomic_int result2{ 0 };

        auto check1 = _receive[0] | [&_result = result1](int x) { _result += x; };
        auto check2 = _receive[0] | [&_result = result2](int x) { _result += x; };

        _receive[0].set_ready();
        for (int i = 0; i < 10; ++i)
            _send[0](i);

        wait_until_done([&_result1 = result1, &_result2 = result2]() { return _result1 == 45 && _result2 == 45; });

        BOOST_REQUIRE_EQUAL(45, result1);
        BOOST_REQUIRE_EQUAL(45, result2);
    }


    BOOST_AUTO_TEST_CASE(int_channel_int_functor) {
        BOOST_TEST_MESSAGE("int channel int functor");

        std::atomic_int result{ 0 };

        auto check = _receive[0] | [](int x) { return x + x; } | [&_result = result](int x) { _result += x; };

        _receive[0].set_ready();
        for (int i = 0; i < 10; ++i)
            _send[0](i);

        wait_until_done([&_result = result]() { return _result == 90; });
        BOOST_REQUIRE_EQUAL(90, result);
    }


    BOOST_AUTO_TEST_CASE(int_channel_split_int_functor) {
        BOOST_TEST_MESSAGE("int channel int functor");
        std::atomic_int result1{ 0 };
        std::atomic_int result2{ 0 };

        auto check1 = _receive[0] | [](int x) { return x + x; } | [&_result = result1](int x) { _result += x; };
        auto check2 = _receive[0] | [](int x) { return x + x; } | [&_result = result2](int x) { _result += x; };

        _receive[0].set_ready();
        for (int i = 0; i < 10; ++i)
            _send[0](i);

        wait_until_done([&_result1 = result1, &_result2 = result2] { return _result1 == 90 && _result2 == 90; });

        BOOST_REQUIRE_EQUAL(90, result1);
        BOOST_REQUIRE_EQUAL(90, result2);
    }


    BOOST_AUTO_TEST_CASE(int_channel_split_int_functor_async) {
        BOOST_TEST_MESSAGE("int channel int functor asynchronous");
        std::vector<stlab::future<void>> inputs;
        for (auto i = 0; i < 10; ++i)
        {
            inputs.emplace_back(stlab::async(stlab::default_scheduler(),
                [&_send = _send, i]() { _send[0](i); }));
        }

        std::atomic_int result1{ 0 };
        std::atomic_int result2{ 0 };

        auto check1 = _receive[0] | [](int x) { return x + x; } | [&_result = result1](int x) { _result += x; };
        auto check2 = _receive[0] | [](int x) { return x + x; } | [&_result = result2](int x) { _result += x; };

        _receive[0].set_ready();

        wait_until_done([&_result1 = result1, &_result2 = result2] { return _result1 == 90 && _result2 == 90; });

        BOOST_REQUIRE_EQUAL(90, result1);
        BOOST_REQUIRE_EQUAL(90, result2);
    }

BOOST_AUTO_TEST_SUITE_END()


using channel_test_fixture_move_only_1 = channel_test_fixture<std::unique_ptr<int>, 1>;

BOOST_FIXTURE_TEST_SUITE(move_only_channel_void_functor, channel_test_fixture_move_only_1)

    BOOST_AUTO_TEST_CASE(move_only_int_channel_void_functor) {
        BOOST_TEST_MESSAGE("move only int channel void functor");

        std::atomic_int result{ 0 };

        auto check = _receive[0] | [&_result = result](std::unique_ptr<int> x) { _result += *x; };

        _receive[0].set_ready();
        for (int i = 0; i < 10; ++i) {
            auto arg = std::make_unique<int>();
            *arg = 1;
            _send[0](std::move(arg));
        }

        wait_until_done([&_result = result]() { return _result == 10; });
        BOOST_REQUIRE_EQUAL(10, result);
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

BOOST_AUTO_TEST_SUITE_END()