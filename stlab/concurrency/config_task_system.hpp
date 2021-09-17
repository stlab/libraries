/*
    Copyright 2015-2021 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_CONCURRENCY_CONFIG_TASK_SYSTEM_HPP
#define STLAB_CONCURRENCY_CONFIG_TASK_SYSTEM_HPP

#include <stlab/has_include.hpp>

/**************************************************************************************************/

#define STLAB_TASK_SYSTEM_PRIVATE_UNDEFINED() 0
#define STLAB_TASK_SYSTEM_PRIVATE_PORTABLE() 1
#define STLAB_TASK_SYSTEM_PRIVATE_LIBDISPATCH() 2
#define STLAB_TASK_SYSTEM_PRIVATE_EMSCRIPTEN() 3
#define STLAB_TASK_SYSTEM_PRIVATE_INTEL_TBB() 4
#define STLAB_TASK_SYSTEM_PRIVATE_WINDOWS() 5

#define STLAB_TASK_SYSTEM(X) (STLAB_TASK_SYSTEM_PRIVATE_##X() == STLAB_TASK_SYSTEM_PRIVATE_USING())

#if defined(STLAB_FORCE_TASK_SYSTEM_LIBDISPATCH)

    #define STLAB_TASK_SYSTEM_PRIVATE_USING() STLAB_TASK_SYSTEM_PRIVATE_LIBDISPATCH()

#elif defined(STLAB_FORCE_TASK_SYSTEM_EMSRIPTEN)

    #define STLAB_TASK_SYSTEM_PRIVATE_USING() STLAB_TASK_SYSTEM_PRIVATE_EMSCRIPTEN()

#elif defined(STLAB_FORCE_TASK_SYSTEM_WINDOWS)

    #define STLAB_TASK_SYSTEM_PRIVATE_USING() STLAB_TASK_SYSTEM_PRIVATE_WINDOWS()

#elif defined(STLAB_FORCE_TASK_SYSTEM_PORTABLE)

    #define STLAB_TASK_SYSTEM_PRIVATE_USING() STLAB_TASK_SYSTEM_PRIVATE_PORTABLE()

#elif defined(STLAB_FORCE_TASK_SYSTEM_INTEL_TBB)

    #define STLAB_TASK_SYSTEM_PRIVATE_USING() STLAB_TASK_SYSTEM_PRIVATE_INTEL_TBB()

#else

    #if defined(__APPLE__) || STLAB_HAS_INCLUDE(<dispatch/dispatch.h>)

        #define STLAB_TASK_SYSTEM_PRIVATE_USING() STLAB_TASK_SYSTEM_PRIVATE_LIBDISPATCH()

    #elif defined(_MSC_VER)

        #define STLAB_TASK_SYSTEM_PRIVATE_USING() STLAB_TASK_SYSTEM_PRIVATE_WINDOWS()

    #elif STLAB_HAS_INCLUDE(<tbb/task_arena.h>)

        #define STLAB_TASK_SYSTEM_PRIVATE_USING() STLAB_TASK_SYSTEM_PRIVATE_INTEL_TBB()

    #elif defined(__EMSCRIPTEN__) && !defined(__EMSCRIPTEN_PTHREADS)

        #define STLAB_TASK_SYSTEM_PRIVATE_USING() STLAB_TASK_SYSTEM_PRIVATE_EMSCRIPTEN()

    #else

        #define STLAB_TASK_SYSTEM_PRIVATE_USING() STLAB_TASK_SYSTEM_PRIVATE_PORTABLE()

    #endif

#endif

/**************************************************************************************************/

#endif // STLAB_CONCURRENCY_CONFIG_TASK_SYSTEM_HPP

/**************************************************************************************************/
