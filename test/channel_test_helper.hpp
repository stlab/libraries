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
    std::atomic_bool _done;

    channel_test_fixture() 
        : _done{false}
    {
        std::tie(_send, _receive) = stlab::channel<T>(stlab::default_scheduler());
    }

    void undone() { _done = false; }

    void wait_until_done() const {
        while (!_done) {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
    }
};

#endif