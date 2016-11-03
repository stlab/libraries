/*
Copyright 2016 Felix Petriconi
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef _CHANNEL_TEST_HELPER_
#define _CHANNEL_TEST_HELPER_

#include <stlab/channel.hpp>

template <typename T>
class channel_test_fixture
{
public:
    stlab::sender<T> _send;
    stlab::receiver<T> _receive;

    channel_test_fixture() 
    {
        std::tie(_send, _receive) = stlab::channel<T>(stlab::default_scheduler());
    }

    template <typename F>
    void wait_until_done(F&& f) const {
        while (!std::forward<F>(f)()) {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
    }
};

#endif