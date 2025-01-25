/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_CONCURRENCY_IMMEDIATE_EXECUTOR_HPP
#define STLAB_CONCURRENCY_IMMEDIATE_EXECUTOR_HPP

#include <stlab/config.hpp>

#include <type_traits>
#include <utility>

/**************************************************************************************************/

namespace stlab {
inline namespace STLAB_VERSION_NAMESPACE() {

/**************************************************************************************************/

namespace detail {

/**************************************************************************************************/

struct immediate_executor_type {
    template <typename F>
    auto operator()(F&& f) const -> std::enable_if_t<std::is_nothrow_invocable_v<F>> {
        std::forward<F>(f)();
    }
};

/**************************************************************************************************/

} // namespace detail

/**************************************************************************************************/

constexpr auto immediate_executor = detail::immediate_executor_type{};

/**************************************************************************************************/

} // namespace STLAB_VERSION_NAMESPACE()
} // namespace stlab

/**************************************************************************************************/

#endif

/**************************************************************************************************/
