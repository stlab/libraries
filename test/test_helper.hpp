/*
Copyright 2016 Felix Petriconi
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/


#ifndef _TEST_HELPER_HPP_
#define _TEST_HELPER_HPP_

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

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
            ++_current_tasks_in_execution;
            std::thread([&_current_count = _current_tasks_in_execution, _f = std::move(f)]{ _f(); --_current_count; }).detach();
        }

        static int usage_counter() { return _usage_counter.load(); }
        static void reset() { _usage_counter = 0; _current_tasks_in_execution = 0; }
        static int current_tasks_in_execution() { return _current_tasks_in_execution.load(); }

    private:
        const size_t _id = no; // only used for debugging purpose
        static std::atomic_int _usage_counter;
        static std::atomic_int _current_tasks_in_execution;
    };


    class test_exception : public std::exception {
        const std::string _error;

    public:
        explicit test_exception(const std::string& error);

        explicit test_exception(const char* error);

        const char* what() const noexcept override;
    };


    struct test_setup
    {
        test_setup() {
            custom_scheduler<0>::reset();
            custom_scheduler<1>::reset();
        }
    };

    template <typename T>
    struct test_fixture
    {
        test_fixture() {
            custom_scheduler<0>::reset();
            custom_scheduler<1>::reset();
        }

        ~test_fixture() {
		    // work in progress: ensure that all futures have finished before end of 
			// text fixture ends, otherwise boost.test complains about memory leaks
            //sut = stlab::future<T>();
            //while (custom_scheduler<0>::current_tasks_in_execution() && custom_scheduler<1>::current_tasks_in_execution());
        }

        stlab::future<T> sut;

        template <typename... F>
        void wait_until_future_completed(F&... f) {
            (void)std::initializer_list<int>{ (wait_until_future_is_ready(f), 0)... };
        }

        template <typename F>
        auto wait_until_future_r_completed(F& f) {
            auto result = f.get_try();
            while (!result.is_initialized()) {
                result = f.get_try();
            }
            return std::move(result);
        }

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

        template <typename E, typename F>
        static void check_failure(F& f, const char* message) {
            BOOST_REQUIRE_EXCEPTION(f.get_try(), E, ([_m = message](const auto& e) { return std::string(_m) == std::string(e.what()); }));
        }

        template <typename E, typename... F>
        void wait_until_future_fails(F&... f) {
            (void)std::initializer_list<int>{ (wait_until_this_future_fails<E>(f), 0)... };
        }


    private:
        template <typename F>
        void wait_until_future_is_ready(F& f) {
            while (!f.get_try()) {}
        }

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