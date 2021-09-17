
/*
    Copyright 2013 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
/**************************************************************************************************/

#ifndef STLAB_HAS_INCLUDE_HPP
#define STLAB_HAS_INCLUDE_HPP

/**************************************************************************************************/

#if defined(__has_include)
#define STLAB_HAS_INCLUDE(X) __has_include(X)
#else
#define STLAB_HAS_INCLUDE(X) 0
#endif

/**************************************************************************************************/

#endif
