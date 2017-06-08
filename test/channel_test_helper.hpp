/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef _CHANNEL_TEST_HELPER_
#define _CHANNEL_TEST_HELPER_

#include <stlab/concurrency/concurrency.hpp>

#include <queue>
#include <thread>

class manual_scheduler
{
    static std::queue<std::function<void()>> _tasks;

public:
    static void clear() { while (!_tasks.empty()) _tasks.pop(); }

    template <typename F>
    void operator()(F&& f) {
        _tasks.push(std::forward<F>(f));
    }

    static void wait_until_queue_size_of(std::size_t n){
        while (_tasks.size() < n) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    static void run_next_task() {
        if (_tasks.empty()) {
            printf("Function lost\n");
            return;
        }
        auto t = std::move(_tasks.front());
        _tasks.pop();
		stlab::default_executor(std::move(t));
    }
};

struct channel_test_fixture_base
{
    template <typename F>
    void wait_until_done(F&& f) const {
        while (!std::forward<F>(f)()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
};

template <typename T, std::size_t N>
struct channel_test_fixture : channel_test_fixture_base
{
    std::array<stlab::sender<T>, N>  _send;
    std::array<stlab::receiver<T>,N> _receive;

    channel_test_fixture() 
    {
        for (std::size_t i = 0; i < N; i++)
            std::tie(_send[i], _receive[i]) = stlab::channel<T>(stlab::default_executor);
    }
};


template <typename U, typename V>
struct channel_types_test_fixture : channel_test_fixture_base
{
    std::tuple<stlab::sender<U>, stlab::sender<V>>   _send;
    std::tuple<stlab::receiver<U>, stlab::receiver<V>> _receive;

    channel_types_test_fixture()
    {
        std::tie(send<0>(), receive<0>()) = stlab::channel<U>(stlab::default_executor);
        std::tie(send<1>(), receive<1>()) = stlab::channel<V>(stlab::default_executor);
    }

    template<std::size_t I>
    auto& send() { return std::get<I>(_send); }

    template <std::size_t I>
    auto& receive() { return std::get<I>(_receive); }

};


template <typename T1, typename T2>
class channel_combine_test_fixture
{
public:
    stlab::sender<T1> _send1;
    stlab::receiver<T2> _receive1;
    stlab::sender<T2> _send2;
    stlab::receiver<T2> _receive2;

    channel_combine_test_fixture()
    {
        std::tie(_send1, _receive1) = stlab::channel<T1>(stlab::default_executor);
        std::tie(_send2, _receive2) = stlab::channel<T2>(stlab::default_executor);
    }

    template <typename F>
    void wait_until_done(F&& f) const {
        while (!std::forward<F>(f)()) {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
    }
};


template <std::size_t N>
struct sum
{
    int _number_additions{ 0 };
    int _x{ 0 };
    stlab::process_state_scheduled _state{ stlab::await_forever };

    void await(int x)
    {
        _x += x;
        ++_number_additions;
        if (_number_additions == N)
        {
            _state = stlab::yield_immediate;
        }
    }

    int yield()
    {
        auto result = _x;
        _state = stlab::await_forever;
        _number_additions = 0;
        _x = 0;
        return result;
    }

    auto state() const { return _state; }
};

inline stlab::process_state_scheduled await_soon() {
    return std::make_pair(stlab::process_state::await,
    std::chrono::system_clock::now() + std::chrono::seconds(1));
}

struct timed_sum
{
    int _limit;
    int _number_additions{ 0 };
    static std::atomic_int _x;

    timed_sum(int limit = 0) : _limit(limit) { _x = 0; }

    stlab::process_state_scheduled _state{ await_soon() };

    void await(int x) {
        _x += x;
        ++_number_additions;
        if (_limit && _number_additions == _limit)
        {
            _state = stlab::yield_immediate;
        }
    }

    int yield() {
        auto result = _x.load();
        _state = stlab::await_forever;
        _number_additions = 0;
        _x = 0;
        return result;
    }

    static int current_sum() { return _x.load(); }

    auto state() const { return _state; }
};


template <std::size_t N>
struct collector
{
    int _collected_items{ 0 };
    std::vector<int> _c;

    stlab::process_state_scheduled _state{ stlab::await_forever };

    void await(int x)
    {
        _c.push_back(x);
        ++_collected_items;
        if (_collected_items == N)
        {
            _state = stlab::yield_immediate;
        }
    }

    std::vector<int> yield()
    {
        auto result = _c;
        _state = stlab::await_forever;
        _collected_items = 0;
        _c.clear();
        return result;
    }

    auto state() const { return _state; }
};



#endif