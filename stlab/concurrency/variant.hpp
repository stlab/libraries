/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_CONCURRENCY_VARIANT_HPP
#define STLAB_CONCURRENCY_VARIANT_HPP

#include <stlab/concurrency/config.hpp>

#define STLAB_STD_VARIANT               1
#define STLAB_STD_EXPERIMENTAL_VARIANT  2
#define STLAB_BOOST_VARIANT             3

//#include <boost/preprocessor/stringize.hpp>
//#pragma message( "C++Version" BOOST_PP_STRINGIZE(__cplusplus) )

#ifdef __has_include                          // Check if __has_include is present
#  if __has_include(<variant>) && STLAB_CPP_VERSION == 17 // Check for a standard library
#    include<variant>
#    define STLAB_VARIANT STLAB_STD_VARIANT
#  elif __has_include(<experimental/variant>) // Check for an experimental version
#    include <experimental/variant>
#    define STLAB_VARIANT STLAB_STD_EXPERIMENTAL_VARIANT
#  else
#    include <boost/variant.hpp>
#    define STLAB_VARIANT STLAB_BOOST_VARIANT
#  endif
#else
#    include <boost/variant.hpp>
#    define STLAB_VARIANT STLAB_BOOST_VARIANT
#endif

namespace stlab
{
inline namespace v1
{
#if STLAB_VARIANT == STLAB_STD_VARIANT

template <typename...T>
using variant = std::variant<T...>;



template <class T, class... Types>
constexpr T& get(std::variant<Types...>& v) {
    return std::get<T>(v);
};
template <class T, class... Types>
constexpr T&& get(std::variant<Types...>&& v) {
    return std::get<T>(std::move(v));
};

template <class T, class... Types>
constexpr const T& get(const std::variant<Types...>& v) {
    return std::get<T>(v);
};

template <class T, class... Types>
constexpr const T&& get(const std::variant<Types...>&& v) {
    return std::get<T>(std::move(v));
};

template <typename...Types>
constexpr auto index(const std::variant<Types...>& v) {
    return v.index();
}

#elif STLAB_VARIANT == STLAB_STD_EXPERIMENTAL_VARIANT
template<typename...T>
using variant = std::experimental::variant<T...>;

#elif STLAB_VARIANT == STLAB_BOOST_VARIANT
template <typename...T>
using variant = boost::variant<T...>;

template <class T, class... Types>
constexpr T& get(boost::variant<Types...>& v) {
    return boost::get<T>(v);
};

template <class T, std::size_t I, typename... Types>
constexpr T& get(boost::variant<Types...>& v) {
    return boost::get<I>(v);
};

template <class T, class... Types>
constexpr const T& get(const boost::variant<Types...>& v) {
    return boost::get<T>(v);
};

template <class T, std::size_t I, typename... Types>
constexpr const T&& get(const boost::variant<Types...>&& v) {
    return boost::get<I>(std::forward<Types...>(v));
};

template <typename...Types>
constexpr auto index(const boost::variant<Types...>& v) {
    return v.which();
}


#else
#error "No variant!"
#endif


}

} // stlab


#endif