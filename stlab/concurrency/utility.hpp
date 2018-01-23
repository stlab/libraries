/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_CONCURRENCY_UTILITY_HPP
#define STLAB_CONCURRENCY_UTILITY_HPP

#include <condition_variable>
#include <exception>
#include <mutex>
#include <type_traits>
#include <boost/optional.hpp>
#include <stlab/concurrency/future.hpp>
#include <stlab/concurrency/immediate_executor.hpp>

/**************************************************************************************************/

#if 0

#include <thread>

// usefull makro for debugging
#define STLAB_TRACE(S) \
    printf("%s:%d %d %s\n", __FILE__, __LINE__, (int)std::hash<std::thread::id>()(std::this_thread::get_id()), S);

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
    stlab::optional<T> result;
    std::exception_ptr error = nullptr;

    bool flag{false};
    std::condition_variable condition;
    std::mutex m;
    auto hold = std::move(x).recover(immediate_executor, [&](auto&& r) {
        if (r.error())
            error = *std::forward<decltype(r)>(r).error();
        else
            result = std::move(*std::forward<decltype(r)>(r).get_try());

        {
            std::unique_lock<std::mutex> lock{m};
            flag = true;

        condition.notify_one();
        }
    });
    {
        std::unique_lock<std::mutex> lock{m};
        while (!flag) {
        condition.wait(lock);
        }
    }

    if (error)
        std::rethrow_exception(error);

    return std::move(*result);
}

template <typename T>
boost::optional<T> blocking_get(future<T> x,const std::chrono::nanoseconds& timeout) {
    boost::optional<T> result;
    std::exception_ptr error = nullptr;

    bool flag{false};
    std::condition_variable condition;
    std::mutex m;

    auto hold = std::move(x).recover(immediate_executor, [&](auto&& r) {
        if (r.error())
            error = std::forward<decltype(r)>(r).error().value();
        else
            result = std::forward<decltype(r)>(r).get_try().value();

        {
            std::unique_lock<std::mutex> lock{m};
            flag = true;
            /*
                WARNING : Calling `notify_one()` inside the lock is a pessimization because
                it means the code waiting will block aquiring the lock as soon as it wakes up.

                However, if we do the notificiation outside the lock, blocking_get will return
                and we'll be calling a destructed condition variable.
            */
            condition.notify_one();
        }
    });
    
    std::unique_lock<std::mutex> lock{m};
    while (!flag) {
        if(condition.wait_for(lock, timeout) == std::cv_status::timeout) {
            hold.reset();
            return result;
        }
    }

    if (error)
        std::rethrow_exception(error);

    return result;
}

inline void blocking_get(future<void> x) {
    std::exception_ptr error = nullptr;

    bool set{false};
    std::condition_variable condition;
    std::mutex m;
    auto hold = std::move(x).recover(immediate_executor, [&](auto&& r) {
        if (r.error()) error = *std::forward<decltype(r)>(r).error();
        {
            std::unique_lock<std::mutex> lock(m);
            set = true;

        condition.notify_one();
        }
    });
    {
    std::unique_lock<std::mutex> lock(m);
    while (!set) {
        condition.wait(lock);
    }
    }

    if (error) std::rethrow_exception(error);
}

/**************************************************************************************************/

} // namespace v1

/**************************************************************************************************/

} // namespace stlab

#endif //SLABFUTURE_UTILITY_HPP
