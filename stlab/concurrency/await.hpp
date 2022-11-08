/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_CONCURRENCY_AWAIT_HPP
#define STLAB_CONCURRENCY_AWAIT_HPP

#include <stlab/config.hpp>

#include <chrono>
#include <condition_variable>
#include <exception>
#include <mutex>
#include <type_traits>
#include <utility>

#include <stlab/concurrency/future.hpp>
#include <stlab/concurrency/immediate_executor.hpp>
#include <stlab/concurrency/ready_future.hpp>
#include <stlab/memory.hpp>

#if STLAB_TASK_SYSTEM(PORTABLE)
#include <stlab/concurrency/default_executor.hpp>
#endif

/**************************************************************************************************/

namespace stlab {

/**************************************************************************************************/

inline namespace v1 {

/**************************************************************************************************/

/**
    Assumes f _will wait_ and wakes or adds a thread to the thread pool (to the limit) before
    invoking f.
*/
template <class F>
auto invoke_waiting(F&& f) {
#if STLAB_TASK_SYSTEM(PORTABLE)
    if (!detail::pts().wake()) detail::pts().add_thread();
#endif

    std::forward<F>(f)();
}

/**************************************************************************************************/

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

template <class T>
struct _get_optional;

template <class T>
struct _get_optional<stlab::optional<T>> {
    template <class F>
    auto operator()(F&& f) {
        return *std::forward<F>(f);
    }
};

template <>
struct _get_optional<bool> {
    template <class F>
    auto operator()(F&&) {}
};

} // namespace detail

template <typename T>
T await(future<T> x) {
    if (auto result = x.get_try())
        return detail::_get_optional<decltype(result)>{}(std::move(result)); // if ready, done

    std::mutex m;
    std::condition_variable condition;
    bool flag{false};

    auto hold = std::move(x).recover(immediate_executor, [&](future<T>&& r) {
        x = std::move(r);
        {
            std::unique_lock<std::mutex> lock{m};
            flag = true;
            condition.notify_one(); // must notify under lock
        }
    });

#if STLAB_TASK_SYSTEM(PORTABLE)
    if (!detail::pts().wake()) detail::pts().add_thread();

    /*
        If no tasks are available we wait for one tick of the system clock and exponentially
        back off on the wait as long as no tasks are available.
    */

    for (auto backoff{std::chrono::steady_clock::duration{std::chrono::milliseconds{1}}}; true;
         backoff *= 2) {
        {
            std::unique_lock<std::mutex> lock{m};
            if (condition.wait_for(lock, backoff, [&] { return flag; })) {
                break;
            }
        }
        detail::pts().wake(); // try to wake something to unstick.
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
        {
            std::unique_lock<std::mutex> lock{_mutex};
            if (_timed_out) return std::forward<F>(r);
            _result = std::forward<F>(r);
        }
        _condition.notify_one();
        /*
            A valid future<> result is required because the following reduction in
            recover() will attach a continuation - which will crash on an invalid
            future<>.
         */
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
auto await_for(future<T> x, const std::chrono::nanoseconds& timeout) -> future<T> {
    if (x.is_ready()) return x;

#if STLAB_TASK_SYSTEM(PORTABLE)
    if (!detail::pts().wake()) detail::pts().add_thread();
#endif

    auto p = std::make_shared<detail::blocking_get_guarded<T>>();

    auto hold = std::move(x).recover(immediate_executor, [_p = stlab::make_weak_ptr(p)](auto&& r) {
        if (auto p = _p.lock()) return p->set(std::forward<decltype(r)>(r));
        return std::forward<decltype(r)>(r);
    });

    auto result = p->wait_for(timeout);
    return result.valid() ? std::move(result) : std::move(hold);
}

/**************************************************************************************************/

template <typename T>
[[deprecated("Use await instead.")]] T blocking_get(future<T> x) {
    return await(std::move(x));
}

template <class T>
[[deprecated("Use await_for instead.")]] auto blocking_get_for(
    future<T> x, const std::chrono::nanoseconds& timeout) -> future<T> {
    await_for(std::move(x), timeout);
}

template <class T>
[[deprecated("Use await_for instead.")]] auto blocking_get(future<T> x,
                                                           const std::chrono::nanoseconds& timeout)
    -> decltype(x.get_try()) {
    return blocking_get_for(std::move(x), timeout).get_try();
}

/**************************************************************************************************/

} // namespace v1

/**************************************************************************************************/

} // namespace stlab

#endif // STLAB_CONCURRENCY_AWAIT_HPP
