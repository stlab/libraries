/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_CONCURRENCY_READY_FUTURE_HPP
#define STLAB_CONCURRENCY_READY_FUTURE_HPP

#include <exception>
#include <type_traits>
#include <utility>

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

/**************************************************************************************************/

} // namespace v1

/**************************************************************************************************/

} // namespace stlab

#endif // STLAB_CONCURRENCY_READY_FUTURE_HPP
