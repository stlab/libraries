/*-
Copyright 2017 Felix Petriconi
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef SLABFUTURE_SYSTEM_TIMER_HPP
#define SLABFUTURE_SYSTEM_TIMER_HPP

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
#include <memory>
#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_PORTABLE
// REVISIT (sparent) : for testing only
#if 0 && __APPLE__
#include <dispatch/dispatch.h>
#endif
#endif

namespace stlab
{

/**************************************************************************************************/

#if STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_LIBDISPATCH

struct system_timer
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
};

#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_EMSCRIPTEN

#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_WINDOWS

class system_timer
{
public:
    template <typename F>
    void operator()(std::chrono::system_clock::time_point when, F&& f) {

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


class system_timer
{
    using element_t = pair<chrono::system_clock::time_point, std::function<void()>>;
    using queue_t = vector<element_t>;

    queue_t             _timed_queue;
    condition_variable  _condition;
    bool                _stop = false;
    mutex               _timed_queue_mutex;
    thread              _timed_queue_thread;


    struct greater_first
    {
        using result_type = bool;

        template <typename T>
        bool operator()(const T& x, const T& y) {
            return x.first > y.first;
        }
    };

    void timed_queue_run() {
        while (true) {
            function<void()> task;
            {
                lock_t lock(_timed_queue_mutex);

                while (_timed_queue.empty() && !_stop) _condition.wait(lock);
                if (_stop) return;
                while (chrono::system_clock::now() < _timed_queue.front().first) {
                    auto when = _timed_queue.front().first;
                    _condition.wait_until(lock, when);
                    if (_stop) return;
                }
                pop_heap(begin(_timed_queue), end(_timed_queue), greater_first());
                task = move(_timed_queue.back().second);
                _timed_queue.pop_back();
            }

            task();
        }
    }

public:
    task_system() {
        _timed_queue_thread = thread([this] { this->timed_queue_run(); });
    }

    ~task_system() {
        {
            lock_t lock(_timed_queue_mutex);
            _stop = true;
        }
        _condition.notify_one();
        _timed_queue_thread.join();
    }

    template <typename F>
    void operator()(chrono::system_clock::time_point when, F&& f) {
        {
            if (when == chrono::system_clock::time_point::min()) {
                async_(forward<F>(f));
            }
            else {
                lock_t lock(_timed_queue_mutex);
                _timed_queue.emplace_back(when, forward<F>(f));
                push_heap(begin(_timed_queue), end(_timed_queue), greater_first());
                _condition.notify_one();
            }
        }
    }
};

#endif

}


#endif //SLABFUTURE_SYSTEM_TIMER_HPP
