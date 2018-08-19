/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#include <boost/test/unit_test.hpp>

#include <stlab/concurrency/channel.hpp>
#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/future.hpp>

#include <string>

#include "channel_test_helper.hpp"

using namespace stlab;
using namespace channel_test_helper;

using channel_test_fixture_int_1 = channel_test_fixture<int, 1>;

BOOST_FIXTURE_TEST_CASE(int_merge_round_robin_channel_void_functor_one_value, channel_test_fixture_int_1) {
    BOOST_TEST_MESSAGE("int merge round robin channel void functor one value one value");

    std::atomic_int result{0};

    auto check = merge_channel<round_robin_t>(default_executor, [&](int x) { result = x; }, _receive[0]);

    _receive[0].set_ready();
    _send[0](1);

    wait_until_done([&] { return result != 0; });

    BOOST_REQUIRE_EQUAL(1, result);
}

BOOST_FIXTURE_TEST_CASE(int_merge_round_robin_channel_void_functor_one_value_async, channel_test_fixture_int_1) {
    BOOST_TEST_MESSAGE("int merge round robin channel void functor one value asynchronously");

    std::atomic_int result{0};

    auto check = merge_channel<round_robin_t>(default_executor, [&](int x) { result = x; }, _receive[0]);

    _receive[0].set_ready();
    auto f = async(default_executor, [_sender = _send[0]] { _sender(1); });

    wait_until_done([&]() { return result != 0; });

    BOOST_REQUIRE_EQUAL(1, result);
}

BOOST_FIXTURE_TEST_CASE(int_merge_round_robin_channel_void_functor_many_values, channel_test_fixture_int_1) {
    BOOST_TEST_MESSAGE("int merge round robin channel void functor many values");

    std::atomic_int result{0};

    auto check = merge_channel<round_robin_t>(default_executor, [&](int x) { result += x; }, _receive[0]);

    _receive[0].set_ready();
    for (auto i = 1; i <= 100; ++i)
        _send[0](i);

    auto expected = 100 * (100 + 1) / 2;

    wait_until_done([&] { return result >= expected; });

    BOOST_REQUIRE_EQUAL(expected, result);
}

BOOST_FIXTURE_TEST_CASE(int_merge_round_robin_channel_void_functor_many_values_async,
                        channel_test_fixture_int_1) {
    BOOST_TEST_MESSAGE("int merge round robin channel void functor many values asynchronously");

    std::atomic_int result{0};

    auto check = merge_channel<round_robin_t>(default_executor, [&](int x) { result += x; }, _receive[0]);

    _receive[0].set_ready();
    std::vector<future<void>> f(100);
    for (auto i = 1; i <= 100; ++i) {
        f.push_back(async(default_executor, [_sender = _send[0], i] { _sender(i); }));
    }

    auto expected = 100 * (100 + 1) / 2;
    wait_until_done([&] { return result >= expected; });

    BOOST_REQUIRE_EQUAL(expected, result);
}

using channel_test_fixture_int_2 = channel_test_fixture<int, 2>;

BOOST_FIXTURE_TEST_CASE(int_merge_round_robin_channel_same_type_void_functor_one_value,
                        channel_test_fixture_int_2) {
    BOOST_TEST_MESSAGE("int merge round robin channel same type void functor oane value");

    std::atomic_int result{0};
    int incrementer{1};
    auto check = merge_channel<round_robin_t>(default_executor,
                     [&](int x) {
                         result += incrementer * x;
                         ++incrementer;
                     },
                     _receive[0], _receive[1]);

    _receive[0].set_ready();
    _receive[1].set_ready();
    _send[0](2);
    _send[1](3);

    auto expectation = 2 + 2 * 3;
    wait_until_done([&] { return result >= expectation; });

    BOOST_REQUIRE_EQUAL(expectation, result);
}

BOOST_FIXTURE_TEST_CASE(int_merge_round_robin_channel_same_type_void_functor_one_value_async,
                        channel_test_fixture_int_2) {
    BOOST_TEST_MESSAGE("int merge round robin channel same type void functor one value asynchronously");

    std::atomic_int result{0};
    int incrementer{1};
    auto check = merge_channel<round_robin_t>(default_executor,
                     [&](int x) {
                         result += incrementer * x;
                         ++incrementer;
                     },
                     _receive[0], _receive[1]);

    _receive[0].set_ready();
    _receive[1].set_ready();
    auto f =
        async(default_executor, [_send1 = _send[0], &_send2 = _send[1]] { // one copy,one reference
            _send1(2);
            _send2(3);
        });

    auto expectation = 2 + 2 * 3;
    wait_until_done([&] { return result >= expectation; });

    BOOST_REQUIRE_EQUAL(expectation, result);
}

BOOST_FIXTURE_TEST_CASE(int_merge_round_robin_channel_same_type_void_functor_many_values,
                        channel_test_fixture_int_2) {
    BOOST_TEST_MESSAGE("int merge round robin channel same type void functor many values");

    std::atomic_int result{0};
    int incrementer{1};
    auto check = merge_channel<round_robin_t>(default_executor,
                     [&](int x) {
                         result += incrementer * x;
                         ++incrementer;
                     },
                     _receive[0], _receive[1]);

    _receive[0].set_ready();
    _receive[1].set_ready();
    for (auto i = 0; i < 10; i++) {
        _send[0](i);
        _send[1](i + 1);
    }

    auto expectation = 0 * 1 + 1 * 2 + 1 * 3 + 2 * 4 + 2 * 5 + 3 * 6 + 3 * 7 + 4 * 8 + 4 * 9 +
                       5 * 10 + 5 * 11 + 6 * 12 + 6 * 13 + 7 * 14 + 7 * 15 + 8 * 16 + 8 * 17 +
                       9 * 18 + 9 * 19 + 10 * 20;
    wait_until_done([&] { return result >= expectation; });

    BOOST_REQUIRE_EQUAL(expectation, result);
}

using channel_test_fixture_pair_2 = channel_test_fixture<std::pair<int, std::size_t>, 5>;

BOOST_FIXTURE_TEST_CASE(int_merge_round_robin_channel_same_type_void_functor_many_values_async,
                        channel_test_fixture_pair_2) {
    BOOST_TEST_MESSAGE("int merge round robin channel same type void functor many values asynchronously");

    std::atomic_int result{0};
    std::atomic_bool zipped_ok{true};
    std::size_t expected_input{0};

    auto check = merge_channel<round_robin_t>(default_executor,
                     [&](std::pair<int, std::size_t> x) {
                         result += x.first;
                         zipped_ok = zipped_ok && (x.second == expected_input);
                         expected_input = expected_input == 1 ? 0 : 1;
                     },
                     _receive[0], _receive[1]);

    _receive[0].set_ready();
    _receive[1].set_ready();
    std::vector<future<void>> f(200);
    for (auto i = 1; i <= 200; i += 2) {
        f.push_back(async(default_executor, [& _send1 = _send[0], _i = i] {
            _send1(std::make_pair(_i, std::size_t(0)));
        }));
        f.push_back(async(default_executor, [& _send2 = _send[1], _i = i] {
            _send2(std::make_pair(_i + 1, std::size_t(1)));
        }));
    }

    auto expectation = 200 * (200 + 1) / 2;

    wait_until_done([&] { return result >= expectation; });

    BOOST_REQUIRE_EQUAL(true, zipped_ok);
    BOOST_REQUIRE_EQUAL(expectation, result);
}

using channel_test_fixture_int_5 = channel_test_fixture<int, 5>;

BOOST_FIXTURE_TEST_CASE(int_merge_round_robin_channel_same_type_void_functor, channel_test_fixture_int_5) {
    BOOST_TEST_MESSAGE("int merge round robin channel same type void functor");

    std::atomic_int result{0};
    std::atomic_int incrementer{1};

    auto check = merge_channel<round_robin_t>(default_executor,
                     [&](int x) {
                         result += incrementer * x;
                         ++incrementer;
                     },
                     _receive[0], _receive[1], _receive[2], _receive[3], _receive[4]);

    for (auto& r : _receive)
        r.set_ready();

    auto i = 2;
    for (auto& s : _send)
        s(i++);

    const auto expected = 2 * 1 + 3 * 2 + 4 * 3 + 5 * 4 + 6 * 5;
    wait_until_done([&] { return result >= expected; });

    BOOST_REQUIRE_EQUAL(expected, result);
}

BOOST_FIXTURE_TEST_CASE(int_merge_round_robin_channel_same_type_void_functor_async, channel_test_fixture_int_5) {
    BOOST_TEST_MESSAGE("int merge round robin channel same type void functor asynchronous");

    std::atomic_int result{0};
    std::atomic_int incrementer{1};

    auto check = merge_channel<round_robin_t>(default_executor,
                     [&](int x) {
                         result += incrementer * x;
                         ++incrementer;
                     },
                     _receive[0], _receive[1], _receive[2], _receive[3], _receive[4]);

    for (auto& r : _receive)
        r.set_ready();

    std::vector<future<void>> f(5);
    for (auto i = 0; i < 5; i++) {
        f.push_back(async(default_executor, [_send = _send[i], _i = i] { _send(_i + 2); }));
    }

    const auto expected = 2 * 1 + 3 * 2 + 4 * 3 + 5 * 4 + 6 * 5;
    wait_until_done([&] { return result >= expected; });

    BOOST_REQUIRE_EQUAL(expected, result);
}
