/*
Copyright 2016 Felix Petriconi
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <stlab/channel.hpp>
#include <string>

#include "channel_test_helper.hpp"

using namespace stlab;

BOOST_FIXTURE_TEST_SUITE(int_channel_join_void_functor, channel_test_fixture<int>)

    BOOST_AUTO_TEST_CASE(int_channel_1_join_void_functor_one_value) {
        BOOST_TEST_MESSAGE("int channel 1 join void functor one value");

        std::atomic_int result{ 0 };

        auto check = join(default_scheduler(), [&_result = result](int x) { _result = x; }, _receive);

        _receive.set_ready();
        _send(1);

        wait_until_done([&_result = result]() { return _result != 0; });

        BOOST_REQUIRE_EQUAL(1, result);
    }

    BOOST_AUTO_TEST_CASE(int_channel_1_join_void_functor_one_value_async) {
        BOOST_TEST_MESSAGE("int channel 1 join void functor one value asynchronously");

        std::atomic_int result{ 0 };

        auto check = join(default_scheduler(), [&_result = result](int x) { _result = x; }, _receive);

        _receive.set_ready();
        auto f = async(default_scheduler(), [_sender = _send]{ _sender(1); });

        wait_until_done([&_result = result]() { return _result != 0; });

        BOOST_REQUIRE_EQUAL(1, result);
    }

    BOOST_AUTO_TEST_CASE(int_channel_1_join_void_functor_many_values) {
        BOOST_TEST_MESSAGE("int channel 1 join void functor many values");

        std::atomic_int result{ 0 };

        auto check = join(default_scheduler(), [&_result = result](int x) { _result += x; }, _receive);

        _receive.set_ready();
        for (auto i = 0; i < 10; ++i)
            _send(i);

        wait_until_done([&_result = result]() { return _result >= 45; });

        BOOST_REQUIRE_EQUAL(45, result);
    }

    BOOST_AUTO_TEST_CASE(int_channel_1_join_void_functor_many_values_async) {
        BOOST_TEST_MESSAGE("int channel 1 join void functor many values asynchronously");

        std::atomic_int result{ 0 };

        auto check = join(default_scheduler(), [&_result = result](int x) { _result += x; }, _receive);

        _receive.set_ready();
        std::vector<future<void>> f(10);
        for (auto i = 0; i < 10; ++i) {
            f.push_back(async(default_scheduler(), [_sender = _send, _i = i]{ _sender(_i); }));
        }

        wait_until_done([&_result = result]() { return _result >= 45; });

        BOOST_REQUIRE_EQUAL(45, result);
    }

    BOOST_AUTO_TEST_CASE(int_channel_2_join_same_type_void_functor) {
        BOOST_TEST_MESSAGE("int channel 2 join same type void functor");

        std::atomic_int result{ 0 };
        sender<int> send2;
        receiver<int> receive2;
        std::tie(send2, receive2) = channel<int>(default_scheduler());

        auto check = join(default_scheduler(),
                             [&_result = result](int x, int y) { _result +=  2*x + 3*y; }, _receive, receive2);

        _receive.set_ready();
        receive2.set_ready();
        _send(2);
        send2(3);

        wait_until_done([&_result = result]() { return _result != 0; });

        BOOST_REQUIRE_EQUAL(13, result);
    }

    BOOST_AUTO_TEST_CASE(int_channel_2_join_same_type_void_functor_many_values) {
        BOOST_TEST_MESSAGE("int channel 2 join same type void functor many values");

        std::atomic_int result{ 0 };
        sender<int> send2;
        receiver<int> receive2;
        std::tie(send2, receive2) = channel<int>(default_scheduler());

        auto check = join(default_scheduler(),
                                 [&_result = result](int x, int y) { _result +=  2*x + 3*y; }, _receive, receive2);

        _receive.set_ready();
        receive2.set_ready();
        for (auto i = 0; i < 10; i++) {
            _send(i);
            send2(i+1);
        }

        wait_until_done([&_result = result]() { return _result >= 255; });

        BOOST_REQUIRE_EQUAL(3 + 2+6 + 4+9 + 6+12 + 8+15 + 10+18 + 12+21 + 14+24 + 16+27 + 18+30 , result);
    }

    BOOST_AUTO_TEST_CASE(int_channel_5_join_same_type_void_functor) {
        BOOST_TEST_MESSAGE("int channel 5 join same type void functor");

        std::atomic_int result{ 0 };
        sender<int> send2, send3, send4, send5;
        receiver<int> receive2, receive3, receive4, receive5;
        std::tie(send2, receive2) = channel<int>(default_scheduler());
        std::tie(send3, receive3) = channel<int>(default_scheduler());
        std::tie(send4, receive4) = channel<int>(default_scheduler());
        std::tie(send5, receive5) = channel<int>(default_scheduler());

        auto check = join(default_scheduler(),
                     [&_result = result](int v, int w, int x, int y, int z) {
                         _result +=  2*v + 3*w + 4*x + 5*y + 6*z; },
                     _receive, receive2, receive3, receive4, receive5);

        _receive.set_ready();
        receive2.set_ready();
        receive3.set_ready();
        receive4.set_ready();
        receive5.set_ready();

        _send(2);
        send2(3);
        send3(4);
        send4(5);
        send5(6);

        wait_until_done([&_result = result]() { return _result != 0; });

        BOOST_REQUIRE_EQUAL(4+9+16+25+36, result);
    }

    BOOST_AUTO_TEST_CASE(int_channel_2_join_different_type_void_functor) {
        BOOST_TEST_MESSAGE("int channel 2 join different type void functor");

        std::atomic_int result{ 0 };
        sender<std::string> send2;
        receiver<std::string> receive2;
        std::tie(send2, receive2) = channel<std::string>(default_scheduler());

        auto check = join(default_scheduler(),
                                 [&_result = result](int x, std::string y) { _result +=  2 + y.size(); }, _receive, receive2);

        _receive.set_ready();
        receive2.set_ready();
        _send(2);
        send2(std::string("Foo"));

        wait_until_done([&_result = result]() { return _result != 0; });

        BOOST_REQUIRE_EQUAL(5, result);
    }

BOOST_AUTO_TEST_SUITE_END()
