/*
    Copyright 2015-2021 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_CONCURRENCY_CONFIG_TASK_SYSTEM_HPP
#define STLAB_CONCURRENCY_CONFIG_TASK_SYSTEM_HPP

/**************************************************************************************************/

#define STLAB_TASK_SYSTEM_PRIVATE_PORTABLE() 0
#define STLAB_TASK_SYSTEM_PRIVATE_LIBDISPATCH() 0
#define STLAB_TASK_SYSTEM_PRIVATE_EMSCRIPTEN() 0
#define STLAB_TASK_SYSTEM_PRIVATE_PNACL() 0
#define STLAB_TASK_SYSTEM_PRIVATE_WINDOWS() 0

#define STLAB_TASK_SYSTEM(X) (STLAB_TASK_SYSTEM_PRIVATE_##X())

#if defined(STLAB_FORCE_TASK_SYSTEM_LIBDISPATCH)

    #undef STLAB_TASK_SYSTEM_PRIVATE_LIBDISPATCH
    #define STLAB_TASK_SYSTEM_PRIVATE_LIBDISPATCH() 1

#elif defined(STLAB_FORCE_TASK_SYSTEM_EMSRIPTEN)

    #undef STLAB_TASK_SYSTEM_PRIVATE_EMSCRIPTEN
    #define STLAB_TASK_SYSTEM_PRIVATE_EMSCRIPTEN() 1

#elif defined(STLAB_FORCE_TASK_SYSTEM_PNACL)

    #undef STLAB_TASK_SYSTEM_PRIVATE_PNACL
    #define STLAB_TASK_SYSTEM_PRIVATE_PNACL() 1

#elif defined(STLAB_FORCE_TASK_SYSTEM_WINDOWS)

    #undef STLAB_TASK_SYSTEM_PRIVATE_WINDOWS
    #define STLAB_TASK_SYSTEM_PRIVATE_WINDOWS() 1

#elif defined(STLAB_FORCE_TASK_SYSTEM_PORTABLE)

    #undef STLAB_TASK_SYSTEM_PRIVATE_PORTABLE
    #define STLAB_TASK_SYSTEM_PRIVATE_PORTABLE() 1

#else

    #if __APPLE__

        #undef STLAB_TASK_SYSTEM_PRIVATE_LIBDISPATCH
        #define STLAB_TASK_SYSTEM_PRIVATE_LIBDISPATCH() 1

    #elif __EMSCRIPTEN__

        #undef STLAB_TASK_SYSTEM_PRIVATE_EMSCRIPTEN
        #define STLAB_TASK_SYSTEM_PRIVATE_EMSCRIPTEN() 1

    #elif __pnacl__

        #undef STLAB_TASK_SYSTEM_PRIVATE_PNACL
        #define STLAB_TASK_SYSTEM_PRIVATE_PNACL() 1

    #elif _MSC_VER

        #undef STLAB_TASK_SYSTEM_PRIVATE_WINDOWS
        #define STLAB_TASK_SYSTEM_PRIVATE_WINDOWS() 1

    #else

        #undef STLAB_TASK_SYSTEM_PRIVATE_PORTABLE
        #define STLAB_TASK_SYSTEM_PRIVATE_PORTABLE() 1

    #endif

#endif

/**************************************************************************************************/

#endif // STLAB_CONCURRENCY_CONFIG_TASK_SYSTEM_HPP

/**************************************************************************************************/
