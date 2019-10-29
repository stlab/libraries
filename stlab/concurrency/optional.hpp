/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_CONCURRENCY_OPTIONAL_HPP
#define STLAB_CONCURRENCY_OPTIONAL_HPP

#include <stlab/concurrency/config.hpp>

/**************************************************************************************************/

#define STLAB_OPTIONAL_PRIVATE_STD() 0
#define STLAB_OPTIONAL_PRIVATE_STD_EXPERIMENTAL() 1
#define STLAB_OPTIONAL_PRIVATE_BOOST() 2

/**************************************************************************************************/

#if defined(STLAB_FORCE_BOOST_OPTIONAL)
    #define STLAB_OPTIONAL_PRIVATE_SELECTION() STLAB_OPTIONAL_PRIVATE_BOOST()
#elif defined(__has_include) // Check if __has_include is present
    #if __has_include(<optional>) && STLAB_CPP_VERSION_AT_LEAST(17)
        #include <optional>
        #define STLAB_OPTIONAL_PRIVATE_SELECTION() STLAB_OPTIONAL_PRIVATE_STD()
    #elif __has_include(<experimental/optional>)
        #include <experimental/optional>
        #define STLAB_OPTIONAL_PRIVATE_SELECTION() STLAB_OPTIONAL_PRIVATE_STD_EXPERIMENTAL()
    #endif
#else
    #define STLAB_OPTIONAL_PRIVATE_SELECTION() STLAB_OPTIONAL_PRIVATE_BOOST()
#endif

#define STLAB_OPTIONAL(X) (STLAB_OPTIONAL_PRIVATE_SELECTION() == STLAB_OPTIONAL_PRIVATE_##X())

/**************************************************************************************************/
// The library can be used with boost::optional, std::experimental::optional or std::optional.
// Without any additional define, it uses the versions from the standard, if it is available.
//
// If using of boost::optional shall be enforced, define STLAB_FORCE_BOOST_OPTIONAL.

#if STLAB_OPTIONAL(BOOST)
    #include <boost/optional.hpp>
#endif

/**************************************************************************************************/

namespace stlab {

/**************************************************************************************************/

#if STLAB_OPTIONAL(STD)

template <typename T>
using optional = std::optional<T>;

constexpr std::nullopt_t nullopt{std::nullopt};

/**************************************************************************************************/

#elif STLAB_OPTIONAL(STD_EXPERIMENTAL)

template <typename T>
using optional = std::experimental::optional<T>;

constexpr std::experimental::nullopt_t nullopt{std::experimental::nullopt};

/**************************************************************************************************/

#elif STLAB_OPTIONAL(BOOST)

template <typename T>
using optional = boost::optional<T>;

const boost::none_t nullopt((boost::none_t::init_tag()));

/**************************************************************************************************/

#else

    #error `optional` variant not specified

#endif

/**************************************************************************************************/

} // namespace stlab

/**************************************************************************************************/

#endif

/**************************************************************************************************/
