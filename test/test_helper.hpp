/*
Copyright 2016 Felix Petriconi
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef _TEST_HELPER_HPP_
#define _TEST_HELPER_HPP_

#include <thread>
#include <atomic>

namespace test_helper
{
    template <size_t no>
    struct custom_scheduler {
        using result_type = void;

        template <typename F>
        void operator()(F f) {
            ++_usage_counter;
             std::thread(std::move(f)).detach();
        }

        static int usage_counter() { return _usage_counter.load(); }
        static void reset_usage_counter() { _usage_counter = 0; }

    private:
        static std::atomic_int _usage_counter;
    };

    struct test_setup
    {
        test_setup() {
            custom_scheduler<0>::reset_usage_counter();
            custom_scheduler<1>::reset_usage_counter();
        }
    };
}

#endif