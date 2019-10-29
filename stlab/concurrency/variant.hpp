/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_CONCURRENCY_VARIANT_HPP
#define STLAB_CONCURRENCY_VARIANT_HPP

#include <stlab/concurrency/config.hpp>

/**************************************************************************************************/

#define STLAB_VARIANT_PRIVATE_STD() 1
#define STLAB_VARIANT_PRIVATE_BOOST() 2

/**************************************************************************************************/

#if defined(STLAB_FORCE_BOOST_VARIANT)
    #define STLAB_VARIANT_PRIVATE_SELECTION() STLAB_VARIANT_PRIVATE_BOOST()
#elif defined(__has_include) // Check if __has_include is present
    #if __has_include(<variant>) && STLAB_CPP_VERSION_AT_LEAST(17)
        #include <variant>
        #define STLAB_VARIANT_PRIVATE_SELECTION() STLAB_VARIANT_PRIVATE_STD()
    #endif
#else
    #define STLAB_VARIANT_PRIVATE_SELECTION() STLAB_VARIANT_PRIVATE_BOOST()
#endif

#define STLAB_VARIANT(X) (STLAB_VARIANT_PRIVATE_SELECTION() == STLAB_VARIANT_PRIVATE_##X())

/**************************************************************************************************/

// The library can be used with boost::variant or std::variant.
// Without any additional define, it uses the versions from the standard, if it is available.
//
// If using of boost::variant shall be enforced, define STLAB_FORCE_BOOST_VARIANT.

#if STLAB_VARIANT(BOOST)
#include <boost/variant.hpp>
#endif

/**************************************************************************************************/

namespace stlab {

/**************************************************************************************************/

#if STLAB_VARIANT(STD)

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

#elif STLAB_VARIANT(BOOST)

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

#else

    #error `variant` variant not specified

#endif

/**************************************************************************************************/

} // namespace stlab

/**************************************************************************************************/

#endif // STLAB_CONCURRENCY_VARIANT_HPP

/**************************************************************************************************/
