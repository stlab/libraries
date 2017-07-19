/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_CONCURRENCY_MAIN_EXECUTOR_HPP
#define STLAB_CONCURRENCY_MAIN_EXECUTOR_HPP

#include "config.hpp"

#include <chrono>
#include <functional>

#if STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_LIBDISPATCH
#include <dispatch/dispatch.h>
#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_EMSCRIPTEN
#include <stlab/concurrency/default_executor.hpp>
#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_PNACL
#include <ppapi/cpp/module.h>
#include <ppapi/cpp/core.h>
#include <ppapi/cpp/completion_callback.h>
#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_WINDOWS
#include <Windows.h>
#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_PORTABLE
// REVISIT (sparent) : for testing only
#if 0 && __APPLE__
#include <dispatch/dispatch.h>
#endif
#endif

/**************************************************************************************************/

namespace stlab {

/**************************************************************************************************/

inline namespace v1 {

/**************************************************************************************************/

namespace detail {

/**************************************************************************************************/

#if STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_LIBDISPATCH

struct main_executor_type
{
    using result_type = void;

    template <typename F>
    void operator()(F f) {
        using f_t = decltype(f);

        dispatch_async_f(dispatch_get_main_queue(),
                         new f_t(std::move(f)), [](void* f_) {
                    auto f = static_cast<f_t*>(f_);
                    (*f)();
                    delete f;
                });
    }
};


#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_EMSCRIPTEN

using main_executor_type = default_executor_type;

#elif  (STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_PORTABLE) \
    || (STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_PNACL) \
    || (STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_WINDOWS)


// TODO (sparent) : We need a main task scheduler for STLAB_TASK_SYSTEM_WINDOWS

#if STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_PNACL

struct main_executor_type
{
    using result_type = void;

    template <typename F>
    void operator()(F f) {
        using f_t = decltype(f);

        pp::Module::Get()->core()->CallOnMainThread(0,
            pp::CompletionCallback([](void* f_, int32_t) {
                auto f = static_cast<f_t*>(f_);
                (*f)();
                delete f;
            }, new f_t(std::move(f))), 0);
    }
};

#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_WINDOWS

// TODO main_executor_type for Windows 8 / 10
struct main_executor_type
{};

#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_PORTABLE

// TODO (sparent) : provide a scheduler and run-loop - this is provide for testing on mac
struct main_executor_type
{
    using result_type = void;

    #if __APPLE__
    template <typename F>
    void operator()(F f) {
        using f_t = decltype(f);

        ::dispatch_async_f(dispatch_get_main_queue(),
                new f_t(std::move(f)), [](void* f_) {
                    auto f = static_cast<f_t*>(f_);
                    (*f)();
                    delete f;
                });
    }
    #endif // __APPLE__
};

#endif // STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_PNACL

#endif // (STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_PORTABLE) || ...

/**************************************************************************************************/

} // namespace detail

/**************************************************************************************************/

constexpr auto main_executor = detail::main_executor_type{};

/**************************************************************************************************/

} // namespace v1

/**************************************************************************************************/

} // namespace stlab

/**************************************************************************************************/

#endif

/**************************************************************************************************/
