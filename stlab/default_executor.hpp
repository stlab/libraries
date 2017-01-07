/*
Copyright 2016 Adobe
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef SLABFUTURE_DEFAULT_EXECUTOR_HPP
#define SLABFUTURE_DEFAULT_EXECUTOR_HPP

#include "config.hpp"

#include <chrono>
#include <functional>

#if STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_LIBDISPATCH
#include <dispatch/dispatch.h>
#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_EMSCRIPTEN
#include <emscripten.h>
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


namespace stlab
{
namespace detail
{

/**************************************************************************************************/

void async_(std::function<void()>);
void async_(std::chrono::system_clock::time_point, std::function<void()>);

}

/**************************************************************************************************/

#if STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_LIBDISPATCH

struct default_executor
{
    using result_type = void;


    template <typename F>
    void operator()(std::chrono::system_clock::time_point when, F f) {

        using namespace std::chrono;

        if (when == system_clock::time_point()) {
            operator()(std::move(f));
            return;
        }

        using f_t = decltype(f);

        dispatch_after_f(dispatch_time(0, duration_cast<nanoseconds>(when - system_clock::now()).count()),
                         dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0),
                         new f_t(std::move(f)),
                         [](void* f_) {
                             auto f = static_cast<f_t*>(f_);
                             (*f)();
                             delete f;
                         });
    }

    template <typename F>
    void operator()(F f) {
        using f_t = decltype(f);

        dispatch_async_f(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0),
                         new f_t(std::move(f)),
                         [](void* f_) {
                             auto f = static_cast<f_t*>(f_);
                             (*f)();
                             delete f;
                         });
    }
};

#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_EMSCRIPTEN

struct default_executor
{
    using result_type = void;

    template <typename F>
    void operator()(F f) {
        // REVISIT (sparent) : Using a negative timeout may give better performance. Need to test.
        using f_t = decltype(f);

        emscripten_async_call([](void* f_) {
                    auto f = static_cast<f_t*>(f_);
                    (*f)();
                    delete f;
                }, new f_t(std::move(f)), 0);
    }
};


#elif  (STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_PORTABLE) \
    || (STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_PNACL) \
    || (STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_WINDOWS)


struct default_executor
{
    using result_type = void;

    template <typename F>
    void operator()(std::chrono::system_clock::time_point when, F f) {
        detail::async_(when, std::move(f));
    }

    template <typename F>
    void operator()(F f) {
        detail::async_(std::move(f));
    }
};

// TODO (sparent) : We need a main task scheduler for STLAB_TASK_SYSTEM_WINDOWS

#if STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_PNACL

#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_WINDOWS

class task_system
{
public:
    template <typename F>
    void async_(F&& f) {
        auto work = CreateThreadpoolWork(&callback_impl<F>,
            new F(std::forward<F>(f)),
            nullptr);
        if (work == nullptr) {
            throw std::bad_alloc();
        }
        SubmitThreadpoolWork(work);
    }

    template <typename F>
    void async_(std::chrono::system_clock::time_point when, F&& f) {

        auto timer = CreateThreadpoolTimer(&timer_callback_impl<F>,
            new F(std::forward<F>(f)),
            nullptr);
        if (timer == nullptr) {
            throw std::bad_alloc();
        }

        auto file_time = time_point_to_FILETIME(when);

        SetThreadpoolTimer(timer,
            &file_time,
            0,
            0);
    }
private:

    template <typename F>
    static void CALLBACK callback_impl(PTP_CALLBACK_INSTANCE /*instance*/,
        PVOID                 parameter,
        PTP_WORK              /*Work*/) {
        std::unique_ptr<F> f(static_cast<F*>(parameter));
        (*f)();
    }

    template <typename F>
    static void CALLBACK timer_callback_impl(PTP_CALLBACK_INSTANCE /*Instance*/,
        PVOID                 parameter,
        PTP_TIMER             /*timer*/) {
        std::unique_ptr<F> f(static_cast<F*>(parameter));
        (*f)();
    }

    FILETIME time_point_to_FILETIME(const std::chrono::system_clock::time_point& when) {
        FILETIME ft = { 0, 0 };
        SYSTEMTIME st = { 0 };
        time_t t = std::chrono::system_clock::to_time_t(when);
        tm utc_tm;
        if (!gmtime_s(&utc_tm, &t)) {
            st.wSecond = static_cast<WORD>(utc_tm.tm_sec);
            st.wMinute = static_cast<WORD>(utc_tm.tm_min);
            st.wHour = static_cast<WORD>(utc_tm.tm_hour);
            st.wDay = static_cast<WORD>(utc_tm.tm_mday);
            st.wMonth = static_cast<WORD>(utc_tm.tm_mon + 1);
            st.wYear = static_cast<WORD>(utc_tm.tm_year + 1900);
            st.wMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(when.time_since_epoch()).count() % 1000;
            SystemTimeToFileTime(&st, &ft);
        }
        return ft;
    }
};

#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_PORTABLE

#endif

#endif

}

#endif //SLABFUTURE_DEFAULT_EXECUTOR_HPP
