/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

// The library can be used with boost::variant or std::variant. Usage of
// boost::variant is enabled when STLAB_USE_BOOST_CPP17_SHIMS is defined.

/**************************************************************************************************/

#ifndef STLAB_CONCURRENCY_VARIANT_HPP
#define STLAB_CONCURRENCY_VARIANT_HPP

/**************************************************************************************************/

#ifdef STLAB_USE_BOOST_CPP17_SHIMS
    #include <boost/variant.hpp>
#else
    #include <variant>
#endif

/**************************************************************************************************/

namespace stlab {

/**************************************************************************************************/

#ifndef STLAB_USE_BOOST_CPP17_SHIMS

template <typename... T>
using variant = std::variant<T...>;

template <class T, class... Types>
constexpr T& get(std::variant<Types...>& v) {
    return std::get<T>(v);
}

template <class T, class... Types>
constexpr T&& get(std::variant<Types...>&& v) {
    return std::get<T>(std::move(v));
}

template <class T, class... Types>
constexpr const T& get(const std::variant<Types...>& v) {
    return std::get<T>(v);
}

template <class T, class... Types>
constexpr const T&& get(const std::variant<Types...>&& v) {
    return std::get<T>(std::move(v));
}

template <typename... Types>
constexpr auto index(const std::variant<Types...>& v) {
    return v.index();
}

/**************************************************************************************************/

#else

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

#endif

/**************************************************************************************************/

} // namespace stlab

/**************************************************************************************************/

#endif // STLAB_CONCURRENCY_VARIANT_HPP
