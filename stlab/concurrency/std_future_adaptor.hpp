/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_CONCURRENCY_STD_FUTURE_ADAPTOR_HPP
#define STLAB_CONCURRENCY_STD_FUTURE_ADAPTOR_HPP

/**************************************************************************************************/

#include <future>
#include <utility>

#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/future.hpp>

/**************************************************************************************************/

namespace stlab {

/**************************************************************************************************/

inline namespace v1 {

/**************************************************************************************************/

template <typename S, typename T, typename F>
auto future_adaptor(S&& s, std::future<T> x, F&& f) ->
    stlab::future<typename std::decay<decltype(f(x.get()))>::type> {
    return stlab::async(std::forward<S>(s), [](std::future<T> x, F f) mutable {
        return f(x.get());
    }, std::move(x), std::forward<F>(f));
}

template <typename S, typename T>
auto future_adaptor(S&& s, std::future<T> x) {
    return stlab::async(std::forward<S>(s), [](std::future<T> x) mutable {
        return x.get();
    }, std::move(x));
}

template <typename T, typename F>
auto future_adaptor(std::future<T> x, F&& f) {
    return future_adaptor(stlab::default_executor, std::move(x), std::forward<F>(f));
}

template <typename T>
auto future_adaptor(std::future<T> x) {
    return future_adaptor(stlab::default_executor, std::move(x));
}

/**************************************************************************************************/

} // inline namespace v1

/**************************************************************************************************/

} // namespace stlab

/**************************************************************************************************/

#endif // STLAB_CONCURRENCY_STD_FUTURE_ADAPTOR_HPP

/**************************************************************************************************/
