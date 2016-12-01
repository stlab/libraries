/*
Copyright 2016 Felix Petriconi
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef _CHANNEL_TEST_HELPER_
#define _CHANNEL_TEST_HELPER_

#include <stlab/channel.hpp>

struct channel_test_fixture_base
{
    template <typename F>
    void wait_until_done(F&& f) const {
        while (!std::forward<F>(f)()) {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
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
        for (auto i = 0; i < N; i++)
            std::tie(_send[i], _receive[i]) = stlab::channel<T>(stlab::default_scheduler());
    }
};


template <typename U, typename V>
struct channel_types_test_fixture : channel_test_fixture_base
{
    std::tuple<stlab::sender<U>, stlab::sender<V>>   _send;
    std::tuple<stlab::receiver<U>, stlab::receiver<V>> _receive;

    channel_types_test_fixture()
    {
        std::tie(send<0>(), receive<0>()) = stlab::channel<U>(stlab::default_scheduler());
        std::tie(send<1>(), receive<1>()) = stlab::channel<V>(stlab::default_scheduler());
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
        std::tie(_send1, _receive1) = stlab::channel<T1>(stlab::default_scheduler());
        std::tie(_send2, _receive2) = stlab::channel<T2>(stlab::default_scheduler());
    }

    template <typename F>
    void wait_until_done(F&& f) const {
        while (!std::forward<F>(f)()) {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
    }
};


#endif