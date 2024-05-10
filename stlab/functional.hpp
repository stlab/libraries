/*
    Copyright 2017 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_FUNCTIONAL_HPP
#define STLAB_FUNCTIONAL_HPP

/**************************************************************************************************/

#include <stlab/config.hpp>

#include <functional>
#include <type_traits>

/**************************************************************************************************/

namespace stlab {
inline namespace STLAB_VERSION_NAMESPACE() {

/**************************************************************************************************/

template <class T>
struct unwrap_reference {
    using type = T;
};

template <class T>
struct unwrap_reference<std::reference_wrapper<T>> {
    using type = T;
};

template <class T>
using unwrap_reference_t = typename unwrap_reference<T>::type;

/**************************************************************************************************/

template <class T>
struct is_reference_wrapper : std::false_type {};
template <class T>
struct is_reference_wrapper<std::reference_wrapper<T>> : std::true_type {};

template <class T>
constexpr bool is_reference_wrapper_v = is_reference_wrapper<T>::value;

/**************************************************************************************************/

template <typename T>
auto unwrap(T& val) -> T& {
    return val;
}

template <typename T>
auto unwrap(const T& val) -> const T& {
    return val;
}

template <typename T>
auto unwrap(std::reference_wrapper<T>& val) -> T& {
    return val.get();
}

template <typename T>
auto unwrap(const std::reference_wrapper<T>& val) -> const T& {
    return val.get();
}

/**************************************************************************************************/

} // namespace STLAB_VERSION_NAMESPACE()
} // namespace stlab

/**************************************************************************************************/

#endif

/**************************************************************************************************/
