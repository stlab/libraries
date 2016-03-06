/*
Copyright 2016 Felix Petriconi
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef _TEST_HELPER_HPP_
#define _TEST_HELPER_HPP_

#include <thread>
#include <atomic>
#include <exception>
#include <string>

#include <stlab/future.hpp>

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


    class test_exception : public std::exception {
        const std::string _error;

    public:
        explicit test_exception(const std::string& error);

        explicit test_exception(const char* error);

        const char* what() const override;
    };


    struct test_setup
    {
        test_setup() {
            custom_scheduler<0>::reset_usage_counter();
            custom_scheduler<1>::reset_usage_counter();
        }
    };

    template <typename T>
    struct fixture_base
    {
        fixture_base() {
            custom_scheduler<0>::reset_usage_counter();
            custom_scheduler<1>::reset_usage_counter();
        }

        stlab::future<T> sut;

        template <typename... F>
        void wait_until_future_completed(F&... f) {
            (void)std::initializer_list<int>{ (wait_until_future_is_ready(f), 0)... };
        }

    private:
        template <typename F>
        void wait_until_future_is_ready(F& f) {
            while (!f.get_try()) {}
        }
    };

    template <typename T>
    struct success_fixture : public fixture_base<T>
    {
        success_fixture() {}

        void check_valid_future() {}

        void check_valid_future(const std::future<T>& f) {
            BOOST_REQUIRE(f.valid() == true);
            BOOST_REQUIRE(f.error().is_initialized() == false);
        }

        template <typename F, typename... FS>
        void check_valid_future(const F& f, const FS&... fs) {
            BOOST_REQUIRE(f.valid() == true);
            BOOST_REQUIRE(f.error().is_initialized() == false);
            check_valid_future(fs...);
        }


    };

    template <typename T>
    struct failure_fixture : public fixture_base<T>
    {
        failure_fixture() {}

        template <typename E, typename F>
        void check_failure(F& f, const std::string& message) {
            BOOST_REQUIRE_EXCEPTION(f.get_try(), E, ([_m = message](const auto& e) { return std::string(e.what()) == _m; }));
        }

        template <typename E, typename... F>
        void wait_until_future_fails(F&... f) {
            (void)std::initializer_list<int>{ (wait_until_this_future_fails<E>(f), 0)... };
        }

    private:
        template <typename E, typename F>
        void wait_until_this_future_fails(F& f) {
            try {
                while (!f.get_try()) {}
            }
            catch (const E&) {
            }
        }
    };
}

#endif