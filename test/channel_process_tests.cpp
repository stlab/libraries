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

#include <algorithm>
#include <vector>

#include "channel_test_helper.hpp"

using namespace stlab;
using namespace channel_test_helper;

using channel_test_fixture_int_1 = channel_test_fixture<int, 1>;

BOOST_FIXTURE_TEST_SUITE(int_channel_process_void_functor, channel_test_fixture_int_1)

BOOST_AUTO_TEST_CASE(int_channel_process_with_one_step) {
    BOOST_TEST_MESSAGE("int channel process with one step");

    std::atomic_int index{0};
    std::vector<int> results(10, 0);

    auto check = _receive[0] | sum<1>() | [&](int x) {
        results[index] = x;
        ++index;
    };

    _receive[0].set_ready();
    for (auto i = 0; i < 10; ++i)
        _send[0](i);

    wait_until_done([&] { return index == 10; });

    for (auto i = 0; i < 10; ++i) {
        BOOST_REQUIRE_EQUAL(i, results[i]);
    }
}

BOOST_AUTO_TEST_CASE(int_channel_process_with_one_step_async) {
    BOOST_TEST_MESSAGE("int channel process with one step asynchronously");

    std::atomic_int index{0};
    std::vector<int> results(10, 0);

    auto check = _receive[0] | sum<1>() | [&](int x) {
        results[x] = x;
        ++index;
    };

    _receive[0].set_ready();
    std::vector<future<void>> f(10);
    for (auto i = 0; i < 10; ++i) {
        f.push_back(async(default_executor, [_send = _send[0], i] { _send(i); }));
    }

    wait_until_done([&] { return index == 10; });

    for (auto i = 0; i < 10; ++i) {
        BOOST_REQUIRE_EQUAL(true, std::find(results.begin(), results.end(), i) != results.end());
    }
}

BOOST_AUTO_TEST_CASE(int_channel_process_with_two_steps) {
    BOOST_TEST_MESSAGE("int channel process with two steps");

    std::atomic_int index{0};
    std::vector<int> results(5, 0);

    auto check = _receive[0] | sum<2>() | [&](int x) {
        results[index] = x;
        ++index;
    };

    _receive[0].set_ready();
    for (auto i = 0; i < 10; ++i)
        _send[0](i);

    wait_until_done([&] { return index == 5; });

    int expectation[] = {1, 5, 9, 13, 17};
    for (auto i = 0; i < 5; ++i) {
        BOOST_REQUIRE_EQUAL(expectation[i], results[i]);
    }
}

BOOST_AUTO_TEST_CASE(int_channel_process_with_two_steps_async) {
    BOOST_TEST_MESSAGE("int channel process with two steps asynchronously");

    std::atomic_int index{0};
    std::vector<std::vector<int>> results;

    auto check = _receive[0] | collector<2>() | [&](std::vector<int> x) {
        results.push_back(x);
        ++index;
    };

    _receive[0].set_ready();
    std::vector<future<void>> f(10);
    for (auto i = 0; i < 10; ++i) {
        f.push_back(async(default_executor, [_send = _send[0], i] { _send(i); }));
    }

    wait_until_done([&] { return index == 5; });

    std::vector<int> expectations = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    BOOST_REQUIRE_EQUAL(std::size_t(5), results.size());

    for (const auto& c : results) {
        BOOST_REQUIRE_EQUAL(std::size_t(2), c.size());
        for (auto i : c) {
            auto it = std::find(expectations.begin(), expectations.end(), i);
            BOOST_REQUIRE_EQUAL(true, it != expectations.end());
            expectations.erase(it);
        }
    }
}

BOOST_AUTO_TEST_CASE(int_channel_process_with_many_steps) {
    BOOST_TEST_MESSAGE("int channel process with many steps");

    std::atomic_int result{0};

    auto check = _receive[0] | sum<10>() | [&](int x) { result = x; };

    _receive[0].set_ready();
    for (auto i = 0; i < 10; ++i)
        _send[0](i);

    wait_until_done([&] { return result != 0; });

    BOOST_REQUIRE_EQUAL(45, result);
}

BOOST_AUTO_TEST_CASE(int_channel_process_with_many_steps_async) {
    BOOST_TEST_MESSAGE("int channel process with many steps asynchronously");

    std::atomic_int result{0};

    auto check = _receive[0] | sum<10>() | [&](int x) { result = x; };

    _receive[0].set_ready();
    std::vector<future<void>> f(10);
    for (auto i = 0; i < 10; ++i) {
        f.push_back(async(default_executor, [_send = _send[0], i] { _send(i); }));
    }

    wait_until_done([&] { return result != 0; });

    BOOST_REQUIRE_EQUAL(45, result);
}

BOOST_AUTO_TEST_CASE(int_channel_split_process_one_step) {
    BOOST_TEST_MESSAGE("int channel split process one step");

    std::atomic_int index1{0};
    std::vector<int> results1(10, 0);
    std::atomic_int index2{0};
    std::vector<int> results2(10, 0);

    auto check1 = _receive[0] | sum<1>() | [& _index = index1, &_results = results1](int x) {
        _results[x] = x;
        ++_index;
    };
    auto check2 = _receive[0] | sum<1>() | [& _index = index2, &_results = results2](int x) {
        _results[x] = x;
        ++_index;
    };

    _receive[0].set_ready();
    for (auto i = 0; i < 10; ++i)
        _send[0](i);

    wait_until_done([&] { return index1 == 10 && index2 == 10; });

    for (auto i = 0; i < 10; ++i) {
        BOOST_REQUIRE_EQUAL(i, results1[i]);
        BOOST_REQUIRE_EQUAL(i, results2[i]);
    }
}

BOOST_AUTO_TEST_CASE(int_channel_split_process_two_steps) {
    BOOST_TEST_MESSAGE("int channel split process two steps");

    std::atomic_int index1{0};
    std::vector<int> results1(5);
    std::atomic_int index2{0};
    std::vector<int> results2(5);

    auto check1 = _receive[0] | sum<2>() | [& _index = index1, &_results = results1](int x) {
        _results[_index] = x;
        ++_index;
    };
    auto check2 = _receive[0] | sum<2>() | [& _index = index2, &_results = results2](int x) {
        _results[_index] = x;
        ++_index;
    };

    _receive[0].set_ready();
    for (auto i = 0; i < 10; ++i)
        _send[0](i);

    wait_until_done([&] { return index1 == 5 && index2 == 5; });

    int expectation[] = {1, 5, 9, 13, 17};
    for (auto i = 0; i < 5; ++i) {
        BOOST_REQUIRE_EQUAL(expectation[i], results1[i]);
        BOOST_REQUIRE_EQUAL(expectation[i], results2[i]);
    }
}

BOOST_AUTO_TEST_CASE(int_channel_split_process_many_steps) {
    BOOST_TEST_MESSAGE("int channel split process many steps");

    std::atomic_int result1{0};
    std::atomic_int result2{0};

    auto check1 = _receive[0] | sum<10>() | [& _result = result1](int x) { _result = x; };
    auto check2 = _receive[0] | sum<10>() | [& _result = result2](int x) { _result = x; };

    _receive[0].set_ready();
    for (auto i = 0; i < 10; ++i)
        _send[0](i);

    wait_until_done([&] { return result1 != 0 && result2 != 0; });

    BOOST_REQUIRE_EQUAL(45, result1);
    BOOST_REQUIRE_EQUAL(45, result2);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_CASE(int_channel_process_with_two_steps_timed) {
    BOOST_TEST_MESSAGE("int channel process with two steps timed");

    std::atomic_int result{0};
    sender<int> send;
    receiver<int> receive;

    std::tie(send, receive) = channel<int>(manual_scheduler());

    auto check = receive | timed_sum() | [&](int x) { result = x; };

    receive.set_ready();
    send(42);

    manual_scheduler::run_next_task();

    manual_scheduler::wait_until_queue_size_of(1);
    manual_scheduler::run_next_task();

    manual_scheduler::wait_until_queue_size_of(1);
    manual_scheduler::run_next_task();

    manual_scheduler::wait_until_queue_size_of(1);
    manual_scheduler::run_next_task();

    while (result == 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    BOOST_REQUIRE_EQUAL(42, result);
}

BOOST_AUTO_TEST_CASE(int_channel_process_with_two_steps_timed_wo_timeout) {
    BOOST_TEST_MESSAGE("int channel process with two steps timed w/o timeout");

    std::atomic_int result{0};
    sender<int> send;
    receiver<int> receive;

    std::tie(send, receive) = channel<int>(default_executor);

    auto check = receive | timed_sum(2) | [&](int x) { result = x; };

    receive.set_ready();
    send(42);

    while (timed_sum::current_sum() != 42) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    send(43);

    while (result == 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    BOOST_REQUIRE_EQUAL(85, result);
}

namespace {
struct process_with_set_error {
    explicit process_with_set_error(std::atomic_bool& check) : _check(check) {}

    std::atomic_bool& _check;

    void await(int) { throw std::runtime_error{""}; }

    void set_error(std::exception_ptr) { _check = true; }

    int yield() { return 42; }

    auto state() const { return await_forever; }
};
} // namespace

BOOST_AUTO_TEST_CASE(int_channel_process_set_error_is_called_on_upstream_error) {
    BOOST_TEST_MESSAGE("int channel process set_error is called on upstream error");

    std::atomic_bool check{false};
    sender<int> send;
    receiver<int> receive;

    std::tie(send, receive) = channel<int>(default_executor);

    auto result = receive |
                  [](auto v) {
                      throw std::runtime_error{""};
                      return v;
                  } |
                  process_with_set_error{check} | [](int) {};

    receive.set_ready();
    send(42);

    while (!check) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    BOOST_REQUIRE_EQUAL(true, check.load());
}

namespace {
struct process_with_close {
    explicit process_with_close(std::atomic_bool& check) : _check(check) {}

    std::atomic_bool& _check;

    void await(int) { throw std::runtime_error{""}; }

    void close() { _check = true; }

    int yield() { return 42; }

    auto state() const { return await_forever; }
};
} // namespace

BOOST_AUTO_TEST_CASE(int_channel_process_close_is_called_on_upstream_error) {
    BOOST_TEST_MESSAGE("int channel process close is called when an upstream eeror happened");

    std::atomic_bool check{false};
    sender<int> send;
    receiver<int> receive;

    std::tie(send, receive) = channel<int>(default_executor);

    auto result = receive |
                  [](auto v) {
                      throw std::runtime_error{""};
                      return v;
                  } |
                  process_with_close{check} | [](int) {};

    receive.set_ready();
    send(42);

    while (!check) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    BOOST_REQUIRE_EQUAL(true, check.load());
}
