/*
Copyright 2016 Felix Petriconi
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include "channel_test_helper.hpp"

using namespace stlab;

using channel_test_fixture_int_1 = channel_test_fixture<int, 1>;

BOOST_FIXTURE_TEST_CASE(int_merge_channel_void_functor_one_value, channel_test_fixture_int_1) {
    BOOST_TEST_MESSAGE("int merge channel void functor one value one value");

    std::atomic_int result{ 0 };

    auto check = merge(default_scheduler(), [&_result = result](int x) { _result = x; }, _receive[0]);

    _receive[0].set_ready();
    _send[0](1);

    wait_until_done([&_result = result]() { return _result != 0; });

    BOOST_REQUIRE_EQUAL(1, result);
}

BOOST_FIXTURE_TEST_CASE(int_merge_channel_void_functor_one_value_async, channel_test_fixture_int_1) {
    BOOST_TEST_MESSAGE("int merge channel void functor one value asynchronously");

    std::atomic_int result{ 0 };

    auto check = merge(default_scheduler(), [&_result = result](int x) { _result = x; }, _receive[0]);

    _receive[0].set_ready();
    auto f = async(default_scheduler(), [_sender = _send[0]]{ _sender(1); });

    wait_until_done([&_result = result]() { return _result != 0; });

    BOOST_REQUIRE_EQUAL(1, result);
}

BOOST_FIXTURE_TEST_CASE(int_merge_channel_void_functor_many_values, channel_test_fixture_int_1) {
    BOOST_TEST_MESSAGE("int merge channel void functor many values");

    std::atomic_int result{ 0 };

    auto check = merge(default_scheduler(), [&_result = result](int x) { _result += x; }, _receive[0]);

    _receive[0].set_ready();
    for (auto i = 1; i <= 100; ++i)
        _send[0](i);

    auto expected = 100 * (100+1) / 2;

    wait_until_done([&_result = result, _expected = expected]() { return _result >= _expected; });

    BOOST_REQUIRE_EQUAL(expected, result);
}

BOOST_FIXTURE_TEST_CASE(int_merge_channel_void_functor_many_values_async, channel_test_fixture_int_1) {
    BOOST_TEST_MESSAGE("int merge channel void functor many values asynchronously");

    std::atomic_int result{ 0 };

    auto check = merge(default_scheduler(), [&_result = result](int x) { _result += x; }, _receive[0]);

    _receive[0].set_ready();
    std::vector<future<void>> f(100);
    for (auto i = 1; i <= 100; ++i) {
        f.push_back(async(default_scheduler(), [_sender = _send[0], _i = i]{ _sender(_i); }));
    }

    auto expected = 100 * (100+1) / 2;
    wait_until_done([&_result = result, _expected = expected]() { return _result >= _expected; });

    BOOST_REQUIRE_EQUAL(expected, result);
}

using channel_test_fixture_int_2 = channel_test_fixture<int, 2>;

BOOST_FIXTURE_TEST_CASE(int_merge_channel_same_type_void_functor_one_value, channel_test_fixture_int_2) {
    BOOST_TEST_MESSAGE("int merge channel same type void functor oane value");

    std::atomic_int result{ 0 };

    auto check = merge(default_scheduler(),
                      [&_result = result](int x) { _result +=  x; }, _receive[0], _receive[1]);

    _receive[0].set_ready();
    _receive[1].set_ready();
    _send[0](2);
    _send[1](3);

    wait_until_done([&_result = result]() { return _result >= 5; });

    BOOST_REQUIRE_EQUAL(5, result);
}

BOOST_FIXTURE_TEST_CASE(int_merge_channel_same_type_void_functor_one_value_async, channel_test_fixture_int_2) {
    BOOST_TEST_MESSAGE("int merge channel same type void functor one value asynchronously");

    std::atomic_int result{ 0 };

    auto check = merge(default_scheduler(),
                      [&_result = result](int x) { _result +=  x; }, _receive[0], _receive[1]);

    _receive[0].set_ready();
    _receive[1].set_ready();
    auto f=async(default_scheduler(), [_send1 = _send[0], &_send2 = _send[1]]{// one copy,one reference
        _send1(2);
        _send2(3);
    });


    wait_until_done([&_result = result]() { return _result >= 5; });

    BOOST_REQUIRE_EQUAL(5, result);
}

BOOST_FIXTURE_TEST_CASE(int_merge_channel_same_type_void_functor_many_values, channel_test_fixture_int_2) {
    BOOST_TEST_MESSAGE("int merge channel same type void functor many values");

    std::atomic_int result{ 0 };

    auto check = merge(default_scheduler(),
                      [&_result = result](int x) { _result +=  x; }, _receive[0], _receive[1]);

    _receive[0].set_ready();
    _receive[1].set_ready();
    for (auto i = 0; i < 10; i++) {
        _send[0](i);
        _send[1](i+1);
    }

    const auto expected = 0+1 + 1+2 + 2+3 + 3+4 + 4+5 + 5+6 + 6+7 + 7+8 + 8+9 + 9+10;
    wait_until_done([&]() { return result >= expected; });

    BOOST_REQUIRE_EQUAL(expected, result);
}

BOOST_FIXTURE_TEST_CASE(int_merge_channel_same_type_void_functor_many_values_async, channel_test_fixture_int_2) {
    BOOST_TEST_MESSAGE("int merge channel same type void functor many values asynchronously");

    std::atomic_int result{ 0 };

    auto check = merge(default_scheduler(),
                      [&_result = result](int x) { _result +=  x; }, _receive[0], _receive[1]);

    _receive[0].set_ready();
    _receive[1].set_ready();
    std::vector<future<void>> f(20);
    for (auto i = 0; i < 10; i++) {
        f.push_back(async(default_scheduler(), [_send1 = _send[0], _i = i] { _send1(_i); }));
        f.push_back(async(default_scheduler(), [&_send2 = _send[1],_i = i] { _send2(_i+1); }));
    }

    const auto expected = 0+1 + 1+2 + 2+3 + 3+4 + 4+5 + 5+6 + 6+7 + 7+8 + 8+9 + 9+10;
    wait_until_done([&]() { return result >= expected; });

    BOOST_REQUIRE_EQUAL(expected, result);
}

using channel_test_fixture_int_5 = channel_test_fixture<int, 5>;
BOOST_FIXTURE_TEST_CASE(int_merge_channel_same_type_void_functor, channel_test_fixture_int_5) {
    BOOST_TEST_MESSAGE("int merge channel same type void functor");

    std::atomic_int result{ 0 };

    auto check = merge(default_scheduler(),
                      [&_result = result](int x) { _result +=  x; },
                      _receive[0], _receive[1], _receive[2], _receive[3], _receive[4]);

    for (auto& r : _receive)
        r.set_ready();

    auto i = 2;
    for (auto& s : _send)
        s(i++);

    const auto expectation = 2+3+4+5+6;
    wait_until_done([&]() { return result >= expectation; });

    BOOST_REQUIRE_EQUAL(expectation, result);
}

BOOST_FIXTURE_TEST_CASE(int_merge_channel_same_type_void_functor_async, channel_test_fixture_int_5) {
    BOOST_TEST_MESSAGE("int merge channel same type void functor asynchronous");

    std::atomic_int result{ 0 };

    auto check = merge(default_scheduler(),
                      [&_result = result](int x) { _result +=  x; },
                      _receive[0], _receive[1], _receive[2], _receive[3], _receive[4]);

    for (auto& r : _receive)
        r.set_ready();

    std::vector<future<void>> f(5);
    for (auto i = 0; i < 5; i++) {
        f.push_back(async(default_scheduler(), [_send = _send[i], _i = i]{ _send(_i+2); }));
    }

    const auto expectation = 2+3+4+5+6;
    wait_until_done([&]() { return result >= expectation; });

    BOOST_REQUIRE_EQUAL(expectation, result);
}

