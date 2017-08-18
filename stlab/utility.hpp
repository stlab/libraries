/*
    Copyright 2017 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_UTILITY_HPP
#define STLAB_UTILITY_HPP

/**************************************************************************************************/

#include <type_traits>

/**************************************************************************************************/

namespace stlab {

/**************************************************************************************************/

inline namespace v1 {

/**************************************************************************************************/

namespace detail {

template <bool, class T>
struct move_if_helper;

template <class T>
struct move_if_helper<true, T> {
    using type = std::remove_reference_t<T>&&;
};

template <class T>
struct move_if_helper<false, T> {
    using type = std::remove_reference_t<T>&;
};

template <bool P, class T>
using move_if_helper_t = typename move_if_helper<P, T>::type;

} // namespace detail

/**************************************************************************************************/

template <bool P, class T>
constexpr detail::move_if_helper_t<P, T> move_if(T&& t) noexcept {
    return static_cast<detail::move_if_helper_t<P, T>>(t);
}

/**************************************************************************************************/

} // namespace v1

/**************************************************************************************************/

} // namespace stlab

/**************************************************************************************************/

#endif

/**************************************************************************************************/

