/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_TYPE_TRAITS_HPP
#define STLAB_TYPE_TRAITS_HPP

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

template <typename F, typename... Args>
struct is_nothrow_invocable {
    template <typename U>
    static auto test(int)
        -> decltype(noexcept(std::declval<U>()(std::declval<Args>()...)), void(), std::true_type());
    template <typename U>
    static auto test(...) -> std::false_type;

    static constexpr bool value = decltype(test<F>(0))::value;
};

} // namespace v1

} // namespace stlab

#endif // STLAB_TYPE_TRAITS_HPP
