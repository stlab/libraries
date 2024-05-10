/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_CONCURRENCY_READY_FUTURE_HPP
#define STLAB_CONCURRENCY_READY_FUTURE_HPP

#include <stlab/config.hpp>

#include <exception>
#include <type_traits>
#include <utility>

#include <stlab/concurrency/future.hpp>

/**************************************************************************************************/

namespace stlab {
inline namespace STLAB_VERSION_NAMESPACE() {

/**************************************************************************************************/

template <typename T, typename E>
auto make_ready_future(T&& x, E executor) -> future<std::decay_t<T>> {
    auto p = package<std::decay_t<T>(std::decay_t<T>)>(
        std::move(executor), [](auto&& x) { return std::forward<decltype(x)>(x); });
    p.first(std::forward<T>(x));
    return std::move(p.second);
}

template <typename E>
auto make_ready_future(E executor) -> future<void> {
    auto p = package<void()>(std::move(executor), []() {});
    p.first();
    return p.second;
}

namespace detail {

template <class T>
struct _make_exceptional_future {
    template <typename E>
    auto operator()(std::exception_ptr error, E executor) const -> future<T> {
        auto p = package<T(T)>(std::move(executor),
                               [](auto&& a) { return std::forward<decltype(a)>(a); });
        p.first.set_exception(std::move(error));
        return std::move(p.second);
    }
};

template <>
struct _make_exceptional_future<void> {
    template <typename E>
    auto operator()(std::exception_ptr error, E executor) const -> future<void> {
        auto p = package<void()>(std::move(executor), []() {});
        p.first.set_exception(std::move(error));
        return std::move(p.second);
    }
};

} // namespace detail

template <typename T, typename E>
auto make_exceptional_future(std::exception_ptr error, E executor) -> future<T> {
    return detail::_make_exceptional_future<T>{}(std::move(error), std::move(executor));
}

/**************************************************************************************************/

} // namespace STLAB_VERSION_NAMESPACE()
} // namespace stlab

/**************************************************************************************************/

#endif // STLAB_CONCURRENCY_READY_FUTURE_HPP
