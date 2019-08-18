/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_CONCURRENCY_CONFIG_HPP
#define STLAB_CONCURRENCY_CONFIG_HPP

/**************************************************************************************************/

#define STLAB_FEATURE_PRIVATE_OBJC_ARC() 0
#define STLAB_FEATURE_PRIVATE_COROUTINES() 0

#define STLAB_FEATURE(X) (STLAB_FEATURE_PRIVATE_##X())

// __has_feature is a clang specific extension, gcc does not know it
#ifndef __has_feature      // Optional.
#define __has_feature(x) 0 // Compatibility with non-clang compilers.
#endif

/**************************************************************************************************/

#define STLAB_TASK_SYSTEM_PRIVATE_PORTABLE() 0
#define STLAB_TASK_SYSTEM_PRIVATE_LIBDISPATCH() 0
#define STLAB_TASK_SYSTEM_PRIVATE_EMSCRIPTEN() 0
#define STLAB_TASK_SYSTEM_PRIVATE_PNACL() 0
#define STLAB_TASK_SYSTEM_PRIVATE_WINDOWS() 0

#define STLAB_TASK_SYSTEM(X) (STLAB_TASK_SYSTEM_PRIVATE_##X())

#define STLAB_CPP_VERSION_PRIVATE_11() 0
#define STLAB_CPP_VERSION_PRIVATE_14() 0
#define STLAB_CPP_VERSION_PRIVATE_17() 0
#define STLAB_CPP_VERSION_PRIVATE_20() 0
#define STLAB_CPP_VERSION_PRIVATE_23() 0

#define STLAB_CPP_VERSION(X) (STLAB_CPP_VERSION_PRIVATE_##X())

#if __APPLE__

    #undef STLAB_TASK_SYSTEM_PRIVATE_LIBDISPATCH
    #define STLAB_TASK_SYSTEM_PRIVATE_LIBDISPATCH() 1

    #if __cplusplus == 201103L
        #undef STLAB_CPP_VERSION_PRIVATE_11
        #define STLAB_CPP_VERSION_PRIVATE_11() 1
    #elif __cplusplus == 201402L
        #undef STLAB_CPP_VERSION_PRIVATE_14
        #define STLAB_CPP_VERSION_PRIVATE_14() 1
    #elif __cplusplus == 201703L
        #undef STLAB_CPP_VERSION_PRIVATE_17
        #define STLAB_CPP_VERSION_PRIVATE_17() 1
    #else
        #error Unknown version of C++.
    #endif

    #if __has_feature(objc_arc)
        #undef STLAB_FEATURE_PRIVATE_OBJC_ARC
        #define STLAB_FEATURE_PRIVATE_OBJC_ARC() 1
    #endif

#elif __EMSCRIPTEN__

    #undef STLAB_TASK_SYSTEM_PRIVATE_EMSCRIPTEN
    #define STLAB_TASK_SYSTEM_PRIVATE_EMSCRIPTEN() 1

#elif __pnacl__

    #undef STLAB_TASK_SYSTEM_PRIVATE_PNACL
    #define STLAB_TASK_SYSTEM_PRIVATE_PNACL() 1

#elif _MSC_VER

    #undef STLAB_TASK_SYSTEM_PRIVATE_WINDOWS
    #define STLAB_TASK_SYSTEM_PRIVATE_WINDOWS() 1

    #if _MSVC_LANG == 201402L
        #undef STLAB_CPP_VERSION_PRIVATE_14
        #define STLAB_CPP_VERSION_PRIVATE_14() 1
    #elif _MSC_FULL_VER >= 191225830 && _MSVC_LANG == 201703L
        #undef STLAB_CPP_VERSION_PRIVATE_17
        #define STLAB_CPP_VERSION_PRIVATE_17() 1
    #else
        #error Unknown version of C++.
    #endif

#else

    #undef STLAB_TASK_SYSTEM_PRIVATE_PORTABLE
    #define STLAB_TASK_SYSTEM_PRIVATE_PORTABLE() 1

    #if __cplusplus == 201103L
        #undef STLAB_CPP_VERSION_PRIVATE_11
        #define STLAB_CPP_VERSION_PRIVATE_11() 1
    #elif __cplusplus == 201402L
        #undef STLAB_CPP_VERSION_PRIVATE_14
        #define STLAB_CPP_VERSION_PRIVATE_14() 1
    #elif __cplusplus == 201703L
        #undef STLAB_CPP_VERSION_PRIVATE_17
        #define STLAB_CPP_VERSION_PRIVATE_17() 1
    #else
        #error Unknown version of C++.
    #endif

#endif

/**************************************************************************************************/

#endif // STLAB_CONCURRENCY_CONFIG_HPP

/**************************************************************************************************/
