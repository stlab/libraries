/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_CONCURRENCY_VARIANT_HPP
#define STLAB_CONCURRENCY_VARIANT_HPP

/**************************************************************************************************/

#include <boost/variant.hpp>

/**************************************************************************************************/

namespace stlab {

/**************************************************************************************************/

template <typename... T>
using variant = boost::variant<T...>;

template <class T, class... Types>
constexpr T& get(boost::variant<Types...>& v) {
    return boost::get<T>(v);
}

template <class T, std::size_t I, typename... Types>
constexpr T& get(boost::variant<Types...>& v) {
    return boost::get<I>(v);
}

template <class T, class... Types>
constexpr const T& get(const boost::variant<Types...>& v) {
    return boost::get<T>(v);
}

template <class T, std::size_t I, typename... Types>
constexpr const T&& get(const boost::variant<Types...>&& v) {
    return boost::get<I>(std::forward<Types...>(v));
}

template <typename... Types>
constexpr auto index(const boost::variant<Types...>& v) {
    return v.which();
}

/**************************************************************************************************/

} // namespace stlab

/**************************************************************************************************/

#endif // STLAB_CONCURRENCY_VARIANT_HPP

/**************************************************************************************************/
