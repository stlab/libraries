/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_TYPE_TRAITS_HPP
#define STLAB_TYPE_TRAITS_HPP

#include <cstddef>
#include <functional>
#include <type_traits>
#include <utility>

/**
    @brief This file contains type traits included in versions of C++ later than 14. Only the
   minimal items used are included. They are not conditional on the C++ version to avoid ODR
   violations.
 */

namespace stlab {

inline namespace v1 {

/**
    @brief See <a href="http://en.cppreference.com/w/cpp/types/is_invocable">std::is_invocable</a>

    This is necessary for GCC prior to 13.2 which does not implement mangling noexcept_expr.
 */

template <class F, class... Args>
struct is_nothrow_invocable {
    static constexpr bool value = noexcept(std::invoke(std::declval<F>(), std::declval<Args>()...));
};

template <class F>
struct is_nothrow_invocable<std::nullptr_t> {
    static constexpr bool value = false; // Not invocable
};

} // namespace v1

} // namespace stlab

#endif // STLAB_TYPE_TRAITS_HPP
