/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_CONCURRENCY_CONFIG_HPP
#define STLAB_CONCURRENCY_CONFIG_HPP

/**************************************************************************************************/

#define STLAB_TASK_SYSTEM_PORTABLE      0
#define STLAB_TASK_SYSTEM_LIBDISPATCH   1
#define STLAB_TASK_SYSTEM_EMSCRIPTEN    2
#define STLAB_TASK_SYSTEM_PNACL         3
#define STLAB_TASK_SYSTEM_WINDOWS       4

#if __APPLE__

#ifndef STLAB_TASK_SYSTEM
#define STLAB_TASK_SYSTEM STLAB_TASK_SYSTEM_LIBDISPATCH
#endif

#elif __EMSCRIPTEN__

#ifndef STLAB_TASK_SYSTEM
#define STLAB_TASK_SYSTEM STLAB_TASK_SYSTEM_EMSCRIPTEN
#endif

#elif __pnacl__

#ifndef STLAB_TASK_SYSTEM
#define STLAB_TASK_SYSTEM STLAB_TASK_SYSTEM_PNACL
#endif

#elif _MSC_VER

#ifndef STLAB_TASK_SYSTEM
#define STLAB_TASK_SYSTEM STLAB_TASK_SYSTEM_WINDOWS
#endif

#endif

// Default configuration

#ifndef STLAB_TASK_SYSTEM
#define STLAB_TASK_SYSTEM STLAB_TASK_SYSTEM_PORTABLE
#endif

/**************************************************************************************************/

#endif

/**************************************************************************************************/
