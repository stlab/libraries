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

#include <stlab/memory.hpp>
#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/future.hpp>
#include <stlab/concurrency/immediate_executor.hpp>
#include <stlab/concurrency/optional.hpp>

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
namespace detail {

struct shared_state {
    bool flag{false};
    std::condition_variable condition;
    std::mutex m;
    std::exception_ptr error{nullptr};
};

template <typename T>
struct shared_state_result : shared_state {
    stlab::optional<T> result;
};
} // namespace detail

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

template <typename T, typename E>
future<T> make_exceptional_future(std::exception_ptr error, E executor) {
    auto p = package<T(T)>(std::move(executor), [_error = error](auto&& x) {
        std::rethrow_exception(_error);
        return std::forward<decltype(x)>(x);
    });
    p.first(T{});
    return std::move(p.second);
}

template <typename E>
future<void> make_exceptional_future(std::exception_ptr error, E executor) {
    auto p = package<void()>(std::move(executor), [_error = error]() {
        std::rethrow_exception(_error);
    });
    p.first();
    return p.second;
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
                          std::chrono::steady_clock::duration::min() :
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

template <typename T>
auto blocking_get(future<T> x, const std::chrono::nanoseconds& timeout) {
    future<T> guarded;
    std::condition_variable condition;
    std::mutex m;

    auto hold = std::move(x).recover(immediate_executor, [&](auto&& r) {
        std::unique_lock<std::mutex> lock{m};
        guarded = std::forward<decltype(r)>(r);
        condition.notify_one();
    });

    bool valid;
    {
        std::unique_lock<std::mutex> lock{m};
        valid = condition.wait_for(lock, timeout, [&] { return guarded.valid(); });
    }

    return valid ? guarded.get_try() : decltype(guarded.get_try()){};
}

/**************************************************************************************************/

} // namespace v1

/**************************************************************************************************/

} // namespace stlab

#endif // SLABFUTURE_UTILITY_HPP
