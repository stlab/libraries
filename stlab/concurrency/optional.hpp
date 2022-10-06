/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

// The library can be used with boost::optional or std::optional. Usage of
// boost::optional is enabled when STLAB_USE_BOOST_CPP17_SHIMS is defined.

/**************************************************************************************************/

#ifndef STLAB_CONCURRENCY_OPTIONAL_HPP
#define STLAB_CONCURRENCY_OPTIONAL_HPP

#include <stlab/config.hpp>

/**************************************************************************************************/

#include <stlab/config.hpp>

#ifdef STLAB_USE_BOOST_CPP17_SHIMS
    #include <boost/optional.hpp>
#else
    #include <optional>
#endif

/**************************************************************************************************/

namespace stlab {

/**************************************************************************************************/

#ifndef STLAB_USE_BOOST_CPP17_SHIMS

template <typename T>
using optional = std::optional<T>;

constexpr std::nullopt_t nullopt{std::nullopt};

/**************************************************************************************************/

#else

template <typename T>
using optional = boost::optional<T>;

const boost::none_t nullopt((boost::none_t::init_tag()));

#endif

/**************************************************************************************************/

} // namespace stlab

/**************************************************************************************************/

#endif // STLAB_CONCURRENCY_OPTIONAL_HPP
