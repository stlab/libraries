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

using lock_t = std::unique_lock<std::mutex>;

namespace test_helper
{
    template <size_t no>
    struct custom_scheduler {
        using result_type = void;

        template <typename F>
        void operator()(F f) {
            ++_usage_counter;
#ifdef WIN32
            stlab::default_scheduler()(std::move(f));
#else
            std::thread(std::move(f)).detach();
#endif
        }

        static int usage_counter() { return _usage_counter.load(); }
        static void reset() { _usage_counter = 0; }

    private:
        const size_t _id = no; // only used for debugging purpose
        static std::atomic_int _usage_counter;
    };


    class test_exception : public std::exception {
        const std::string _error;

    public:
        test_exception() {}

        explicit test_exception(const std::string& error);

        explicit test_exception(const char* error);

        test_exception& operator=(const test_exception&) = default;

        test_exception(const test_exception&) = default;

        virtual ~test_exception() {}

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

        ~test_fixture() {}

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
        static void check_failure(F&& f, const char* message) {
            BOOST_REQUIRE_EXCEPTION(std::forward<F>(f).get_try(), E, ([_m = message](const auto& e) { return std::string(_m) == std::string(e.what()); }));
        }

        template <typename E, typename... F>
        void wait_until_future_fails(F&... f) {
            (void)std::initializer_list<int>{ (wait_until_this_future_fails<E>(f), 0)... };
        }


    private:
        template <typename F>
        void wait_until_future_is_ready(F& f) {
            while (!f.get_try()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        }

        template <typename E, typename F>
        void wait_until_this_future_fails(F& f) {
            try {
                while (!f.get_try()) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
            }
            catch (const E&) {
            }
        }
    };

    struct thread_block_context
    {
        std::shared_ptr<std::mutex> _mutex;
        std::condition_variable     _thread_block;
        std::atomic_bool            _go{false};
        std::atomic_bool            _may_proceed{ false };

        thread_block_context()
            : _mutex(std::make_shared<std::mutex>())
        {}
    };

    template <typename R>
    class blocking_functor
    {
        thread_block_context& _context;
        std::atomic_int& _functor_counter;
        R _result;
    public:
        using result_type = R;

        blocking_functor(thread_block_context& context, std::atomic_int& functor_counter)
            : _context(context)
            , _functor_counter(functor_counter)
        {}

        blocking_functor returns(const R& result) {
            _result = result;
            return *this;
        }

        R operator()() {
            lock_t lock(*_context._mutex);
    
            while (!_context._go || !_context._may_proceed) {
                _context._thread_block.wait(lock);
            }
            ++_functor_counter;
            return _result;
        }
    };

    template <typename R, typename... Args>
    auto make_blocking_functor(Args&&... args) {
        return blocking_functor<R>(std::forward<Args>(args)...);
    }
}

#endif