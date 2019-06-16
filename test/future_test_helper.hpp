/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef FUTURE_TEST_HELPER_HPP_
#define FUTURE_TEST_HELPER_HPP_

#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/future.hpp>

#include <atomic>
#include <boost/test/unit_test.hpp>
#include <exception>
#include <string>
#include <thread>

using lock_t = std::unique_lock<std::mutex>;

namespace future_test_helper {

template <std::size_t no>
struct custom_scheduler {
    using result_type = void;

    void operator()(stlab::task<void()> f) const {
        ++counter();
        // The implementation on Windows or the mac uses a scheduler that allows many tasks in the
        // pool in parallel
#if defined(WIN32) || defined(__APPLE__)
        stlab::default_executor(std::move(f));
#else
        // The default scheduler under Linux allows only as many tasks as there are physical cores.
        // But this can lead to a dead lock in some of the tests
        std::thread(std::move(f)).detach();
#endif
    }

    static int usage_counter() { return counter().load(); }

    static void reset() {
        counter() = 0;
    }

    static std::atomic_int& counter() {
        static std::atomic_int counter;
        return counter;
    }

private:
    size_t _id = no; // only used for debugging purpose
};



template <std::size_t I>
stlab::executor_t make_executor() {
    return [_executor = custom_scheduler<I>{}](stlab::task<void()> f) mutable {
        _executor(std::move(f));
    };
}


class test_exception : public std::exception {
    std::string _error;

public:
    test_exception() {}

    explicit test_exception(const std::string& error);

    explicit test_exception(const char* error);

    test_exception& operator=(const test_exception&) = default;
    test_exception(const test_exception&) = default;
    test_exception& operator=(test_exception&&) = default;
    test_exception(test_exception&&) = default;

    virtual ~test_exception() {}

    const char* what() const noexcept override;
};

struct test_setup {
    test_setup() {
        custom_scheduler<0>::reset();
        custom_scheduler<1>::reset();
    }
};

template <typename T>
struct test_fixture {
    test_fixture() : _task_counter{0} {
        custom_scheduler<0>::reset();
        custom_scheduler<1>::reset();
    }

    ~test_fixture() {}

    stlab::future<T> sut;

    template <typename... F>
    void wait_until_future_completed(F&... f) {
        (void)std::initializer_list<int>{(wait_until_future_is_ready(f), 0)...};
    }

    template <typename F>
    auto wait_until_future_r_completed(F& f) {
        auto result = f.get_try();
        while (!result) {
            result = f.get_try();
        }
        return std::move(result);
    }

    void check_valid_future() {}

    void check_valid_future(const stlab::future<T>& f) {
        BOOST_REQUIRE(f.valid() == true);
        BOOST_REQUIRE(!f.exception());
    }

    template <typename F, typename... FS>
    void check_valid_future(const F& f, const FS&... fs) {
        BOOST_REQUIRE(f.valid() == true);
        BOOST_REQUIRE(!f.exception());
        check_valid_future(fs...);
    }

    template <typename E, typename F>
    static void check_failure(F& f, const char* message) {
        BOOST_REQUIRE_EXCEPTION(f.get_try(), E, ([_m = message](const auto& e) {
                                    return std::string(_m) == std::string(e.what());
                                }));
    }

    template <typename E, typename... F>
    void wait_until_future_fails(F&... f) {
        (void)std::initializer_list<int>{(wait_until_this_future_fails<E>(f), 0)...};
    }

    void wait_until_all_tasks_completed() {
        while (_task_counter.load() != 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    std::atomic_int _task_counter;

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
        } catch (const E&) {
        }
    }
};

struct thread_block_context {
    std::shared_ptr<std::mutex> _mutex;
    std::condition_variable _thread_block;
    std::atomic_bool _go{false};
    std::atomic_bool _may_proceed{false};

    thread_block_context() : _mutex(std::make_shared<std::mutex>()) {}
};

class scoped_decrementer {
    std::atomic_int& _v;

public:
    explicit scoped_decrementer(std::atomic_int& v) : _v(v) {}

    ~scoped_decrementer() { --_v; }
};

template <typename F, typename P>
class test_functor_base : public P {
    F _f;
    std::atomic_int& _task_counter;

public:
    test_functor_base(F f, std::atomic_int& task_counter)
        : _f(std::move(f)), _task_counter(task_counter) {}

    ~test_functor_base() {}

    test_functor_base(const test_functor_base&) = default;
    test_functor_base& operator=(const test_functor_base&) = default;
    test_functor_base(test_functor_base&&) = default;
    test_functor_base& operator=(test_functor_base&&) = default;

    template <typename... Args>
    auto operator()(Args&&... args) const {
        ++_task_counter;
        scoped_decrementer d(_task_counter);
        P::action();
        return _f(std::forward<Args>(args)...);
    }
};

struct null_policy {
    void action() const {}
};

class blocking_policy {
    thread_block_context* _context{nullptr};

public:
    void set_context(thread_block_context* context) { _context = context; }

    void action() const {
        lock_t lock(*_context->_mutex);

        while (!_context->_go || !_context->_may_proceed) {
            _context->_thread_block.wait(lock);
        }
    }
};

class failing_policy {
public:
    void action() const { throw test_exception("failure"); }
};

template <typename F>
auto make_non_blocking_functor(F&& f, std::atomic_int& task_counter) {
    return test_functor_base<F, null_policy>(std::forward<F>(f), task_counter);
}

template <typename F>
auto make_blocking_functor(F&& f, std::atomic_int& task_counter, thread_block_context& context) {
    auto result = test_functor_base<F, blocking_policy>(std::forward<F>(f), task_counter);
    result.set_context(&context);
    return result;
}

template <typename F>
auto make_failing_functor(F&& f, std::atomic_int& task_counter) {
    return test_functor_base<F, failing_policy>(std::forward<F>(f), task_counter);
}
} // namespace future_test_helper

#endif
