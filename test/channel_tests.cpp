/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#include <boost/test/unit_test.hpp>

#ifndef STLAB_DISABLE_FUTURE_COROUTINES
#define STLAB_DISABLE_FUTURE_COROUTINES
#endif

#include <queue>
#include <stlab/concurrency/channel.hpp>
#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/future.hpp>
#include <stlab/concurrency/immediate_executor.hpp>
#include <stlab/test/model.hpp>

#include "channel_test_helper.hpp"

using namespace stlab;
using namespace std;
using namespace channel_test_helper;

BOOST_AUTO_TEST_CASE(int_sender) {
    BOOST_TEST_MESSAGE("int sender");

    sender<int> send;

    BOOST_REQUIRE_NO_THROW(send(42));
}

BOOST_AUTO_TEST_CASE(int_receiver) {
    BOOST_TEST_MESSAGE("int receiver");

    receiver<int> receive;

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

    atomic_int result{0};

    auto check = _receive[0] | [&](int x) { result = x; };
    auto sut = move(_send[0]);

    _receive[0].set_ready();
    sut(42);

    wait_until_done([&] { return result == 42; });

    BOOST_REQUIRE_EQUAL(42, result);
}

BOOST_AUTO_TEST_CASE(int_channel_move_assignment_sender) {
    BOOST_TEST_MESSAGE("int channel move assignment sender");

    sender<int> sut;
    atomic_int result{0};

    auto check = _receive[0] | [&](int x) { result = x; };
    sut = move(_send[0]);

    _receive[0].set_ready();
    sut(42);

    wait_until_done([&] { return result == 42; });

    BOOST_REQUIRE_EQUAL(42, result);
}

BOOST_AUTO_TEST_CASE(int_channel_copy_ctor_sender) {
    BOOST_TEST_MESSAGE("int channel copy ctor sender");

    atomic_int result{0};

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

    sender<int> sut;
    atomic_int result{0};

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

    atomic_int result{0};

    auto sut = move(_receive[0]);

    auto check = sut | [&](int x) { result = x; };
    sut.set_ready();
    _send[0](42);

    wait_until_done([&] { return result == 42; });

    BOOST_REQUIRE_EQUAL(42, result);
}

BOOST_AUTO_TEST_CASE(int_channel_move_assignment_receiver) {
    BOOST_TEST_MESSAGE("int channel move assignment receiver");

    receiver<int> sut;
    atomic_int result{0};

    sut = move(_receive[0]);
    auto check = sut | [&](int x) { result = x; };

    sut.set_ready();
    _send[0](42);

    wait_until_done([&] { return result == 42; });

    BOOST_REQUIRE_EQUAL(42, result);
}

BOOST_AUTO_TEST_CASE(int_channel_copy_ctor_receiver) {
    BOOST_TEST_MESSAGE("int channel copy ctor receiver");

    atomic_int result{0};

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

    receiver<int> sut;
    atomic_int result{0};

    sut = _receive[0];

    auto check = sut | [&](int x) { result = x; };

    _receive[0].set_ready();
    sut.set_ready();

    _send[0](42);

    wait_until_done([&] { return result == 42; });

    BOOST_REQUIRE_EQUAL(42, result);
}

BOOST_AUTO_TEST_CASE(int_concatenate_two_channels) {
    atomic_int result{0};
    sender<int> A, X;
    receiver<int> B, Y;
    tie(A, B) = channel<int>(default_executor);
    tie(X, Y) = channel<int>(default_executor);

    auto b = B | [](int x) { return x * 2; } | X;

    auto y = Y | [](int v) { return v + 2; } | [&](int v) { result = v; };

    B.set_ready();
    Y.set_ready();

    A(20);

    wait_until_done([&] { return result == 42; });

    BOOST_REQUIRE_EQUAL(42, result);
}
BOOST_AUTO_TEST_SUITE_END()

namespace {

class main_queue {
    deque<task<void()>> _q;

public:
    auto executor() {
        return [this](auto&& task) { _q.emplace_back(forward<decltype(task)>(task)); };
    }

    void run() {
        while (execute_next_task()) {
        }
    }

    bool execute_next_task() {
        if (!_q.empty()) {
            _q.front()();
            _q.pop_front();
            return true;
        }
        return false;
    }
};

struct echo {
    process_state_scheduled _state = await_forever;
    int _result = -1;

    void await(int x) {
        _result = x;
        _state = yield_immediate;
    }

    int yield() {
        _state = await_forever;
        return _result;
    }

    void close() {}

    const auto& state() const { return _state; }
};

template <typename... T>
struct generator {
    const process_state_scheduled _state = yield_immediate;

    int _value = 0;
    tuple<T&...> _queues;

    generator(T&... q) : _queues(q...) {}

    template <size_t... I>
    void push_values(int value, index_sequence<I...>) {
        (void)initializer_list<int>{(get<I>(_queues).push(value), 0)...};
    }

    int yield() {
        push_values(_value, make_index_sequence<sizeof...(T)>());
        return _value++;
    }

    void close() {}

    const auto& state() const { return _state; }
};

void RequireInClosedRange(size_t minValue, size_t test, size_t maxValue) {
    BOOST_REQUIRE_LE(minValue, test);
    BOOST_REQUIRE_LE(test, maxValue);
}

} // namespace

BOOST_AUTO_TEST_CASE(int_channel_with_2_sized_buffer) {
    main_queue q;

    auto receive = channel<void>(q.executor());
    queue<int> valuesInFlight;

    generator myGenerator(valuesInFlight);
    echo myEcho;

    auto r2 = move(receive) | ref(myGenerator) |
              (buffer_size{2} & ref(myEcho)) | [&valuesInFlight](auto x) {
                  BOOST_REQUIRE_EQUAL(x, valuesInFlight.front());
                  valuesInFlight.pop();
                  cout << x << endl;
              };

    r2.set_ready();

    // The first buffer has size 2 and the next process has per default size 1,
    // so there can be max 2 + 1 values in flight
    q.execute_next_task(); // generate value(0)
    BOOST_REQUIRE_GE(3u, valuesInFlight.size());
    q.execute_next_task(); // await and yield value(0) by echo
    BOOST_REQUIRE_GE(3u, valuesInFlight.size());

    q.execute_next_task(); // generate value(1)
    BOOST_REQUIRE_GE(3u, valuesInFlight.size());
    q.execute_next_task(); // generate value(2)
    RequireInClosedRange(2u, valuesInFlight.size(), 3u);

    q.execute_next_task(); // print value (0)
    RequireInClosedRange(2u, valuesInFlight.size(), 3u);
    q.execute_next_task(); // generate value(3)
    RequireInClosedRange(2u, valuesInFlight.size(), 3u);
    q.execute_next_task(); // await and yield value(1) by echo
    RequireInClosedRange(2u, valuesInFlight.size(), 3u);
    q.execute_next_task(); // print value (1)
    RequireInClosedRange(2u, valuesInFlight.size(), 3u);
    q.execute_next_task(); // await and yield value(2) by echo
    RequireInClosedRange(2u, valuesInFlight.size(), 3u);
    q.execute_next_task(); // generate value(4)
    RequireInClosedRange(2u, valuesInFlight.size(), 3u);
    q.execute_next_task(); // print value (2)
    RequireInClosedRange(2u, valuesInFlight.size(), 3u);
    q.execute_next_task(); // await and yield value(3) by echo
    RequireInClosedRange(2u, valuesInFlight.size(), 3u);
    q.execute_next_task(); // generate value(5)
    RequireInClosedRange(2u, valuesInFlight.size(), 3u);
    q.execute_next_task(); // print value (3)
    RequireInClosedRange(2u, valuesInFlight.size(), 3u);
}

BOOST_AUTO_TEST_CASE(int_channel_with_3_sized_buffer) {
    main_queue q;

    auto receive = channel<void>(q.executor());
    queue<int> valuesInFlight;

    generator myGenerator(valuesInFlight);
    echo myEcho;

    auto r2 = move(receive) | ref(myGenerator) |
              (buffer_size{3} & ref(myEcho)) | [&valuesInFlight](auto x) {
                  BOOST_REQUIRE_EQUAL(x, valuesInFlight.front());
                  valuesInFlight.pop();
              };

    r2.set_ready();

    // The first buffer has size 3 and the next process has per default size 1,
    // so there can be max 3 + 1 values in flight

    q.execute_next_task(); // generate value(0)
    BOOST_REQUIRE_GE(3u, valuesInFlight.size());
    q.execute_next_task(); // await and yield value(0) by echo
    BOOST_REQUIRE_GE(3u, valuesInFlight.size());
    q.execute_next_task(); // generate value(1)
    BOOST_REQUIRE_GE(3u, valuesInFlight.size());
    q.execute_next_task(); // generate value(2)

    RequireInClosedRange(2u, valuesInFlight.size(), 4u);
    q.execute_next_task(); // print value (0)
    RequireInClosedRange(2u, valuesInFlight.size(), 4u);
    q.execute_next_task(); // generate value(3)
    RequireInClosedRange(2u, valuesInFlight.size(), 4u);
    q.execute_next_task(); // generate value(4)
    RequireInClosedRange(2u, valuesInFlight.size(), 4u);
    q.execute_next_task(); // await and yield value(1) by echo
    RequireInClosedRange(2u, valuesInFlight.size(), 4u);
    q.execute_next_task(); // print value (1)
    RequireInClosedRange(2u, valuesInFlight.size(), 4u);
    q.execute_next_task(); // await and yield value(2) by echo
    RequireInClosedRange(2u, valuesInFlight.size(), 4u);
    q.execute_next_task(); // generate value(5)
    RequireInClosedRange(2u, valuesInFlight.size(), 4u);
    q.execute_next_task(); // print value (2)
    RequireInClosedRange(2u, valuesInFlight.size(), 4u);
    q.execute_next_task(); // await and yield value(3) by echo
    RequireInClosedRange(2u, valuesInFlight.size(), 4u);
    q.execute_next_task(); // generate value(6)
    RequireInClosedRange(2u, valuesInFlight.size(), 4u);
    q.execute_next_task(); // print value (3)
    RequireInClosedRange(2u, valuesInFlight.size(), 4u);
}

BOOST_AUTO_TEST_CASE(int_channel_with_split_different_sized_buffer) {
    // Here the bigger buffer size must not steer the upstream, but the
    // smaller size
    vector<pair<size_t, size_t>> bufferSizes = {
        {1, 2}, {1, 2}, {1, 3}, {3, 1}, {2, 1}};

    for (const auto& bs : bufferSizes) {
        main_queue q;

        queue<int> valuesInFlight1;
        queue<int> valuesInFlight2;

        generator myGenerator1(valuesInFlight1);
        generator myGenerator2(valuesInFlight2);

        auto receive = channel<void>(q.executor());

        auto g = move(receive) | generator(valuesInFlight1, valuesInFlight2);

        auto r1 = g | (buffer_size(bs.first) & echo()) | [&valuesInFlight1](auto x) {
            BOOST_REQUIRE_EQUAL(x, valuesInFlight1.front());
            valuesInFlight1.pop();
        };
        auto r2 = g | (buffer_size(bs.second) & echo()) | [&valuesInFlight2](auto x) {
            BOOST_REQUIRE_EQUAL(x, valuesInFlight2.front());
            valuesInFlight2.pop();
        };

        g.set_ready();
        r1.set_ready();
        r2.set_ready();

        // In all cases one buffer has just the size of 1 so the overall number of values
        // in flight can only be 1 + 1

        q.execute_next_task(); // generate value(0)
        RequireInClosedRange(0, valuesInFlight1.size(), 2u);
        RequireInClosedRange(0, valuesInFlight2.size(), 2u);
        q.execute_next_task(); // await and yield value(0) by echo1
        RequireInClosedRange(0, valuesInFlight1.size(), 2u);
        RequireInClosedRange(0, valuesInFlight2.size(), 2u);
        q.execute_next_task(); // await and yield value(0) by echo2
        RequireInClosedRange(0, valuesInFlight1.size(), 2u);
        RequireInClosedRange(0, valuesInFlight2.size(), 2u);
        q.execute_next_task(); // print1 value (0)
        RequireInClosedRange(0, valuesInFlight1.size(), 2u);
        RequireInClosedRange(0, valuesInFlight2.size(), 2u);
        q.execute_next_task(); // generate value(1)
        RequireInClosedRange(0, valuesInFlight1.size(), 2u);
        RequireInClosedRange(0, valuesInFlight2.size(), 2u);
        q.execute_next_task(); // print2 value (0)
        RequireInClosedRange(0, valuesInFlight1.size(), 2u);
        RequireInClosedRange(0, valuesInFlight2.size(), 2u);
        q.execute_next_task(); // await and yield value(1) by echo1
        RequireInClosedRange(0, valuesInFlight1.size(), 2u);
        RequireInClosedRange(0, valuesInFlight2.size(), 2u);
        q.execute_next_task(); // await and yield value(1) by echo2
        RequireInClosedRange(0, valuesInFlight1.size(), 2u);
        RequireInClosedRange(0, valuesInFlight2.size(), 2u);
        q.execute_next_task(); // print1 value (1)
        RequireInClosedRange(0, valuesInFlight1.size(), 2u);
        RequireInClosedRange(0, valuesInFlight2.size(), 2u);
        q.execute_next_task(); // generate value(1)
        q.execute_next_task(); // print2 value (1)
        RequireInClosedRange(0, valuesInFlight1.size(), 2u);
        RequireInClosedRange(0, valuesInFlight2.size(), 2u);
    }
}

BOOST_AUTO_TEST_CASE(int_channel_one_value_different_buffer_sizes) {
    BOOST_TEST_MESSAGE("int channel one value different buffer sizes");

    for (auto bs : {0, 1, 2, 10}) {
        sender<int> send;
        receiver<int> receive;
        tie(send, receive) = channel<int>(default_executor);
        atomic_int result{0};

        auto check = receive | (buffer_size(bs) & [&](int x) { result += x; });

        receive.set_ready();
        send(1);

        while (result < 1) {
            this_thread::sleep_for(chrono::microseconds(1));
        }

        BOOST_REQUIRE_EQUAL(1, result);
    }
}

BOOST_AUTO_TEST_CASE(int_channel_two_values_different_buffer_sizes) {
    BOOST_TEST_MESSAGE("int channel two values different buffer sizes");

    for (auto bs : {0, 1, 2, 10}) {
        sender<int> send;
        receiver<int> receive;
        tie(send, receive) = channel<int>(default_executor);
        atomic_int result{0};

        auto check = receive | (buffer_size(bs) & [&](int x) { result += x; });

        receive.set_ready();
        send(1);
        send(1);

        while (result < 2) {
            this_thread::sleep_for(chrono::microseconds(1));
        }

        BOOST_REQUIRE_EQUAL(2, result);
    }
}

BOOST_AUTO_TEST_CASE(int_channel_many_values_different_buffer_sizes) {
    BOOST_TEST_MESSAGE("int channel many values different buffer sizes");

    for (auto bs : {0, 1, 2, 10}) {
        sender<int> send;
        receiver<int> receive;
        tie(send, receive) = channel<int>(default_executor);
        atomic_int result{0};

        auto check = receive | (buffer_size(bs) & [&](int x) { result += x; });

        receive.set_ready();
        for (auto i = 0; i < 10; ++i)
            send(1);

        while (result < 10) {
            this_thread::sleep_for(chrono::microseconds(1));
        }

        BOOST_REQUIRE_EQUAL(10, result);
    }
}

BOOST_AUTO_TEST_CASE(report_channel_broken_when_no_process_is_attached) {
    BOOST_TEST_MESSAGE("Expect broken channel exception when no process is attached");

    receiver<int> receive;

    BOOST_REQUIRE_EXCEPTION(
        (receive | [](int) { return 1; }), channel_error,
        ([](const auto& e) { return string("broken channel") == e.what(); }));

    BOOST_REQUIRE_EXCEPTION(
        (receive | (buffer_size{2} & [](int) { return 1; })), channel_error,
        ([](const auto& e) { return string("broken channel") == e.what(); }));
}

BOOST_AUTO_TEST_CASE(report_channel_broken_when_process_is_already_running) {
    BOOST_TEST_MESSAGE(
        "Expect \"process already running\" exception when process is already running");

    sender<int> send;
    receiver<int> receive;
    tie(send, receive) = channel<int>(default_executor);

    receive.set_ready();

    BOOST_REQUIRE_EXCEPTION(
        (receive | [](int) { return 1; }), channel_error,
        ([](const auto& e) { return string("process already running") == e.what(); }));

    BOOST_REQUIRE_EXCEPTION(
        (receive | (buffer_size{2} & [](int) { return 1; })), channel_error,
        ([](const auto& e) { return string("process already running") == e.what(); }));
}

BOOST_AUTO_TEST_CASE(sender_receiver_equality_tests) {
    BOOST_TEST_MESSAGE("running sender equality tests");
    {
        sender<int> a, b;
        receiver<int> x, y;

        BOOST_REQUIRE(a == b);
        BOOST_REQUIRE(!(a != b));

        BOOST_REQUIRE(x == y);
        BOOST_REQUIRE(!(x != y));
    }

    {
        sender<move_only> a, b;
        receiver<move_only> x, y;

        BOOST_REQUIRE(a == b);
        BOOST_REQUIRE(!(a != b));

        BOOST_REQUIRE(x == y);
        BOOST_REQUIRE(!(x != y));
    }

    {
        sender<int> send;
        receiver<int> rec;
        tie(send, rec) = channel<int>(immediate_executor);

        auto a = send;
        auto x = rec;
        BOOST_REQUIRE(send == a);
        BOOST_REQUIRE(rec == x);
    }

    {
        sender<int> a, b;
        receiver<int> x, y;
        tie(a, x) = channel<int>(immediate_executor);
        tie(b, y) = channel<int>(immediate_executor);

        BOOST_REQUIRE(a != b);
        BOOST_REQUIRE(x != y);
    }
    {
        sender<move_only> send;
        receiver<move_only> rec;
        tie(send, rec) = channel<move_only>(immediate_executor);

        sender<move_only> a;
        BOOST_REQUIRE(a != send);
    }
}

BOOST_AUTO_TEST_CASE(sender_receiver_swap_tests) {
    {
        sender<int> a, b;
        receiver<int> x, y;
        tie(a, x) = channel<int>(immediate_executor);
        tie(b, y) = channel<int>(immediate_executor);
        int result1(0), result2(0);

        auto v = x | [&result1](int i) { result1 = i; };
        auto w = y | [&result2](int i) { result2 = i; };

        x.set_ready();
        y.set_ready();

        swap(a, b);

        a(1);
        b(2);

        BOOST_REQUIRE_EQUAL(1, result2);
        BOOST_REQUIRE_EQUAL(2, result1);
    }
    {
        sender<int> a, b;
        receiver<int> x, y;
        tie(a, x) = channel<int>(immediate_executor);
        tie(b, y) = channel<int>(immediate_executor);
        int result1(0), result2(0);

        swap(x, y);

        auto v = x | [&result1](int i) { result1 = i; };
        auto w = y | [&result2](int i) { result2 = i; };

        x.set_ready();
        y.set_ready();

        a(1);
        b(2);

        BOOST_REQUIRE_EQUAL(1, result2);
        BOOST_REQUIRE_EQUAL(2, result1);
    }
    {
        sender<move_only> a, b;
        receiver<move_only> x, y;
        tie(a, x) = channel<move_only>(immediate_executor);
        tie(b, y) = channel<move_only>(immediate_executor);
        int result1(0), result2(0);

        auto v = x | [&result1](move_only i) { result1 = i.member(); };
        auto w = y | [&result2](move_only i) { result2 = i.member(); };

        x.set_ready();
        y.set_ready();

        swap(a, b);

        a(1);
        b(2);

        BOOST_REQUIRE_EQUAL(1, result2);
        BOOST_REQUIRE_EQUAL(2, result1);
    }
    {
        sender<move_only> a, b;
        receiver<move_only> x, y;
        tie(a, x) = channel<move_only>(immediate_executor);
        tie(b, y) = channel<move_only>(immediate_executor);
        int result1(0), result2(0);

        swap(x, y);

        auto v = x | [&result1](move_only i) { result1 = i.member(); };
        auto w = y | [&result2](move_only i) { result2 = i.member(); };

        x.set_ready();
        y.set_ready();

        a(1);
        b(2);

        BOOST_REQUIRE_EQUAL(1, result2);
        BOOST_REQUIRE_EQUAL(2, result1);
    }
}
