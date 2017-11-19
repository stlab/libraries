/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_CONCURRENCY_UTILITY_HPP
#define STLAB_CONCURRENCY_UTILITY_HPP

#if 0

#include <thread>

// usefull makro for debugging
#define STLAB_TRACE(S) \
    printf("%s:%d %d %s\n", __FILE__, __LINE__, (int)std::hash<std::thread::id>()(std::this_thread::get_id()), S);

#endif

#include <stlab/concurrency/future.hpp>

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
    return p.second;
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
    return p.second;
}

template <typename T>
T blocking_get(future<T> x) {
    T result;
    std::exception_ptr error;

    bool set{false};
    std::condition_variable condition;
    std::mutex m;
    auto hold = std::move(x).recover([&](auto&& r) {
        {
            std::unique_lock<std::mutex> lock(m);
            if (r.error())
                error = std::forward<decltype(r)>(r).error().value();
            else
                result = std::forward<decltype(r)>(r).get_try().value();
            set = true;
        }
        condition.notify_one();
    });
    std::unique_lock<std::mutex> lock(m);
    while (!set) {
        condition.wait(lock);
    }

    if (error)
        std::rethrow_exception(error);

    return result;
}


inline void blocking_get(future<void> x) {
    std::exception_ptr error;

    bool set{false};
    std::condition_variable condition;
    std::mutex m;
    auto hold = std::move(x).recover([&](auto&& r) {
        {
            std::unique_lock<std::mutex> lock(m);
            if (r.error())
                error = std::forward<decltype(r)>(r).error().value();
            set = true;
        }
        condition.notify_one();
    });
    std::unique_lock<std::mutex> lock(m);
    while (!set) {
        condition.wait(lock);
    }

    if (error)
        std::rethrow_exception(error);
}

/**************************************************************************************************/

} // namespace v1

/**************************************************************************************************/

} // namespace stlab

#endif //SLABFUTURE_UTILITY_HPP
