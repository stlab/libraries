/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_CONCURRENCY_UTILITY_HPP
#define STLAB_CONCURRENCY_UTILITY_HPP

#include <chrono>
#include <condition_variable>
#include <exception>
#include <mutex>
#include <type_traits>

#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/future.hpp>
#include <stlab/concurrency/immediate_executor.hpp>
#include <stlab/memory.hpp>

/**************************************************************************************************/

#if 0

#include <thread>

// usefull makro for debugging
#define STLAB_TRACE(S)                          \
    printf("%s:%d %d %s\n", __FILE__, __LINE__, \
           (int)std::hash<std::thread::id>()(std::this_thread::get_id()), S);

#endif

/**************************************************************************************************/

namespace stlab {

/**************************************************************************************************/

inline namespace v1 {

/**************************************************************************************************/

template <typename T, typename E>
future<std::decay_t<T>> make_ready_future(T&& x, E executor) {
    auto p = package<std::decay_t<T>(std::decay_t<T>)>(
        std::move(executor), [](auto&& x) { return std::forward<decltype(x)>(x); });
    p.first(std::forward<T>(x));
    return std::move(p.second);
}

template <typename E>
future<void> make_ready_future(E executor) {
    auto p = package<void()>(std::move(executor), []() {});
    p.first();
    return p.second;
}

namespace detail {

template <class T>
struct _make_exceptional_future {
    template <typename E>
    future<T> operator()(std::exception_ptr error, E executor) const {
        auto p = package<T(T)>(std::move(executor),
                               [](auto&& a) { return std::forward<decltype(a)>(a); });
        p.first.set_exception(std::move(error));
        return std::move(p.second);
    }
};

template <>
struct _make_exceptional_future<void> {
    template <typename E>
    future<void> operator()(std::exception_ptr error, E executor) const {
        auto p = package<void()>(std::move(executor), []() {});
        p.first.set_exception(std::move(error));
        return std::move(p.second);
    }
};

} // namespace detail

template <typename T, typename E>
future<T> make_exceptional_future(std::exception_ptr error, E executor) {
    return detail::_make_exceptional_future<T>{}(std::move(error), std::move(executor));
}

namespace detail {

template <class T>
struct _get_ready_future {
    template <class F>
    auto operator()(F&& f) {
        return *std::forward<F>(f).get_try();
    }
};

template <>
struct _get_ready_future<void> {
    template <class F>
    void operator()(F&& f) {
        std::forward<F>(f).get_try();
    }
};

} // namespace detail

template <typename T>
T blocking_get(future<T> x) {
    bool flag{false};
    std::condition_variable condition;
    std::mutex m;

    auto hold = std::move(x).recover(immediate_executor, [&](auto&& r) {
        x = std::forward<decltype(r)>(r);

        {
            std::unique_lock<std::mutex> lock{m};
            flag = true;
            condition.notify_one();
        }
    });

    #if STLAB_TASK_SYSTEM(PORTABLE)

    /*
        Steal tasks from the default executor to help avoid deadlocks. We should also do this for
        the single threaded emscripten case but that will require adding a queue to steal
        from.

        If no tasks are available we wait for one tick of the system clock and exponentially
        back off on the wait as long as no tasks are available.
    */

    for (auto backoff{std::chrono::steady_clock::duration::zero()}; true;) {
        {
            std::unique_lock<std::mutex> lock{m};
            if (condition.wait_for(lock, backoff, [&] { return flag; })) break;
        }

        if (detail::pts().steal()) {
            backoff = std::chrono::steady_clock::duration::zero();
        } else {
            backoff = (backoff == std::chrono::steady_clock::duration::zero()) ?
                          backoff = std::chrono::steady_clock::duration{1} :
                          backoff * 2;
        }
    }

    #else

    {
        std::unique_lock<std::mutex> lock{m};
        condition.wait(lock, [&] { return flag; });
    }

    #endif

    return detail::_get_ready_future<T>{}(std::move(x));
}

namespace detail {

template <class T>
struct blocking_get_guarded {
    bool _timed_out{false};
    future<T> _result;
    std::condition_variable _condition;
    std::mutex _mutex;

    template <class F>
    auto set(F&& r) -> future<T> {
        std::unique_lock<std::mutex> lock{_mutex};
        if (_timed_out) return std::forward<F>(r);
        _result = std::forward<F>(r);
        _condition.notify_one();
        return make_exceptional_future<T>(
            std::make_exception_ptr(std::runtime_error("not an error")), immediate_executor);
    }

    auto wait_for(const std::chrono::nanoseconds& timeout) -> future<T> {
        std::unique_lock<std::mutex> lock{_mutex};
        _timed_out = !_condition.wait_for(lock, timeout, [&] { return _result.valid(); });
        return _timed_out ? future<T>{} : std::move(_result);
    }
};

} // namespace detail

template <class T>
auto blocking_get_for(future<T> x, const std::chrono::nanoseconds& timeout) -> future<T> {
    auto p = std::make_shared<detail::blocking_get_guarded<T>>();

    auto hold = std::move(x).recover(immediate_executor, [_p = stlab::make_weak_ptr(p)](auto&& r) {
        if (auto p = _p.lock()) return p->set(std::forward<decltype(r)>(r));
        return make_exceptional_future<T>(
            std::make_exception_ptr(std::runtime_error("not an error")), immediate_executor);
    });

    auto result = p->wait_for(timeout);
    return result.valid() ? std::move(result) : std::move(hold);
}

template <class T>
[[deprecated("Use blocking_get_for instead.")]] auto blocking_get(
    future<T> x, const std::chrono::nanoseconds& timeout) -> decltype(x.get_try()) {
    return blocking_get_for(std::move(x), timeout).get_try();
}

/**************************************************************************************************/

} // namespace v1

/**************************************************************************************************/

} // namespace stlab

#endif // STLAB_CONCURRENCY_UTILITY_HPP
