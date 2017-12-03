/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_CONCURRENCY_OPTIONAL_HPP
#define STLAB_CONCURRENCY_OPTIONAL_HPP

#define STLAB_STD_OPTIONAL               1
#define STLAB_STD_EXPERIMENTAL_OPTIONAL  2
#define STLAB_STD_BOOST_OPTIONAL         3

#ifdef __has_include                           // Check if __has_include is present
#  if __has_include(<optional>)                // Check for a standard library
#    include<optional>
#    define STLAB_OPTIONAL STLAB_STD_OPTIONAL
#  elif __has_include(<experimental/optional>) // Check for an experimental version
#    include <experimental/optional>
#    define STLAB_OPTIONAL STLAB_STD_EXPERIMENTAL_OPTIONAL
#  endif
#else
#    include <boost/optional.hpp>
#    define STLAB_OPTIONAL STLAB_STD_BOOST_OPTIONAL
#endif

namespace stlab
{

#if STLAB_OPTIONAL == STLAB_STD_OPTIONAL

template <typename T>
using optional = std::optional<T>;
constexpr std::nullopt_t nullopt{ std::nullopt_t::_Tag{} };

#elif STLAB_OPTIONAL == STLAB_STD_EXPERIMENTAL_OPTIONAL
template<typename T>
using optional = std::experimental::optional<T>;

constexpr std::experimental::nullopt_t nullopt{0};

#elif STLAB_OPTIONAL == STLAB_STD_BOOST_OPTIONAL
template <typename T>
using optional = boost::optional<T>;

constexpr boost::none_t = nullopt;

#else
#error "No optional!"
#endif

} // stlab


#endif