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

#define STLAB_FEATURE(X) (STLAB_FEATURE_PRIVATE_##X())

// __has_feature is a clang specific extension, gcc does not know it
#ifndef __has_feature      // Optional.
#define __has_feature(x) 0 // Compatibility with non-clang compilers.
#endif

/**************************************************************************************************/

#define STLAB_TASK_SYSTEM_PORTABLE()      0
#define STLAB_TASK_SYSTEM_LIBDISPATCH()   1
#define STLAB_TASK_SYSTEM_EMSCRIPTEN()    2
#define STLAB_TASK_SYSTEM_PNACL()         3
#define STLAB_TASK_SYSTEM_WINDOWS()       4

#if __APPLE__

#ifndef STLAB_TASK_SYSTEM
#define STLAB_TASK_SYSTEM() STLAB_TASK_SYSTEM_LIBDISPATCH()

#if __cplusplus >= 201703L
#define STLAB_CPP_VERSION() 17
#endif

#endif

#if __has_feature(objc_arc)
    #undef STLAB_FEATURE_PRIVATE_OBJC_ARC
    #define STLAB_FEATURE_PRIVATE_OBJC_ARC() 1
#endif

#elif __EMSCRIPTEN__

#ifndef STLAB_TASK_SYSTEM
#define STLAB_TASK_SYSTEM() STLAB_TASK_SYSTEM_EMSCRIPTEN()
#endif

#elif __pnacl__

#ifndef STLAB_TASK_SYSTEM
#define STLAB_TASK_SYSTEM() STLAB_TASK_SYSTEM_PNACL()
#endif

#elif _MSC_VER

#ifndef STLAB_TASK_SYSTEM
#define STLAB_TASK_SYSTEM() STLAB_TASK_SYSTEM_WINDOWS()
#endif

#if _MSC_FULL_VER >= 191225830 && _MSVC_LANG >= 201403L
#define STLAB_CPP_VERSION() 17
#endif

#endif

// Default configuration

#ifndef STLAB_TASK_SYSTEM
#define STLAB_TASK_SYSTEM() STLAB_TASK_SYSTEM_PORTABLE()
#if __cplusplus >= 201703L
#define STLAB_CPP_VERSION() 17
#endif
#endif

#ifndef STLAB_CPP_VERSION
#define STLAB_CPP_VERSION() 17
#endif

#ifndef STLAB_FUTURE_COROUTINES
#define STLAB_FUTURE_COROUTINES 0
#endif

/**************************************************************************************************/

#endif

/**************************************************************************************************/
