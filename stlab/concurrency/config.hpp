/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_CONCURRENCY_CONFIG_HPP
#define STLAB_CONCURRENCY_CONFIG_HPP

#include <stlab/concurrency/config_task_system.hpp>

/**************************************************************************************************/

#define STLAB_FEATURE_PRIVATE_OBJC_ARC() 0
#define STLAB_FEATURE_PRIVATE_COROUTINES() 0

#define STLAB_FEATURE(X) (STLAB_FEATURE_PRIVATE_##X())

/**************************************************************************************************/

#define STLAB_CPP_VERSION(X) (STLAB_CPP_VERSION_PRIVATE() == (X))
#define STLAB_CPP_VERSION_LESS_THAN(X) (STLAB_CPP_VERSION_PRIVATE() < (X))
#define STLAB_CPP_VERSION_AT_LEAST(X) (STLAB_CPP_VERSION_PRIVATE() >= (X))

#if __APPLE__

    #if defined(__has_feature)
        #if __has_feature(objc_arc)
            #undef STLAB_FEATURE_PRIVATE_OBJC_ARC
            #define STLAB_FEATURE_PRIVATE_OBJC_ARC() 1
        #endif
    #endif

#elif _MSC_VER

    #if _MSVC_LANG == 201103L
        #define STLAB_CPP_VERSION_PRIVATE() 11
    #elif _MSVC_LANG == 201402L
        #define STLAB_CPP_VERSION_PRIVATE() 14
    #elif _MSC_FULL_VER >= 191225830 && _MSVC_LANG == 201703L
        #define STLAB_CPP_VERSION_PRIVATE() 17
    #else
        #warning Unknown version of C++, assuming C++20.
        #define STLAB_CPP_VERSION_PRIVATE() 20
    #endif

#endif

#if !defined(STLAB_CPP_VERSION_PRIVATE)
    #if __cplusplus == 201103L
        #define STLAB_CPP_VERSION_PRIVATE() 11
    #elif __cplusplus == 201402L
        #define STLAB_CPP_VERSION_PRIVATE() 14
    #elif __cplusplus == 201703L
        #define STLAB_CPP_VERSION_PRIVATE() 17
    #else
        //#warning Unknown version of C++, assuming C++20.
        #define STLAB_CPP_VERSION_PRIVATE() 20
    #endif
#endif

/**************************************************************************************************/

#endif // STLAB_CONCURRENCY_CONFIG_HPP

/**************************************************************************************************/
