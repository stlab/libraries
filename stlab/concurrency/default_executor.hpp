/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_CONCURRENCY_DEFAULT_EXECUTOR_HPP
#define STLAB_CONCURRENCY_DEFAULT_EXECUTOR_HPP

#include <stlab/concurrency/config.hpp>
#include <stlab/concurrency/task.hpp>

#include <chrono>
#include <functional>

#if STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_LIBDISPATCH
#include <dispatch/dispatch.h>
#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_EMSCRIPTEN
#include <emscripten.h>
#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_PNACL
#include <ppapi/cpp/completion_callback.h>
#include <ppapi/cpp/core.h>
#include <ppapi/cpp/module.h>
#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_WINDOWS
#include <Windows.h>
#include <memory>
#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_PORTABLE
#include <array>
#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <deque>
#include <thread>
#include <vector>

#include <stlab/concurrency/task.hpp>

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

enum class executor_priority
{
    high,
    medium,
    low
};

/**************************************************************************************************/

#if STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_LIBDISPATCH

constexpr auto platform_priority(executor_priority p)
{
    switch (p)
    {
        case executor_priority::low:
            return DISPATCH_QUEUE_PRIORITY_LOW;
        case executor_priority::medium:
            return DISPATCH_QUEUE_PRIORITY_DEFAULT;
        case executor_priority::high:
            return DISPATCH_QUEUE_PRIORITY_HIGH;
    }
}


template <executor_priority P = executor_priority::medium>
struct executor_type {
private:
    struct group {
        dispatch_group_t _group = dispatch_group_create();
        ~group() {
            dispatch_group_wait(_group, DISPATCH_TIME_FOREVER);
#if !STLAB_FEATURE(OBJC_ARC)
            dispatch_release(_group);
#endif
        }
    };

public:

    using result_type = void;
    template <typename F>
    void operator()(F f) const {
        using f_t = decltype(f);

        static group g;

        dispatch_group_async_f(g._group,
                               dispatch_get_global_queue(platform_priority(P), 0),
                               new f_t(std::move(f)), [](void* f_) {
                                   auto f = static_cast<f_t*>(f_);
                                   (*f)();
                                   delete f;
                               });
    }
};

#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_EMSCRIPTEN

template <executor_priority P = executor_priority::medium>
struct executor_type {
    using result_type = void;

    template <typename F>
    void operator()(F f) const {
        // REVISIT (sparent) : Using a negative timeout may give better performance. Need to test.
        using f_t = decltype(f);

        emscripten_async_call(
            [](void* f_) {
                auto f = static_cast<f_t*>(f_);
                (*f)();
                delete f;
            },
            new f_t(std::move(f)), 0);
    }
};

#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_PNACL

template <executor_priority P = executor_priority::medium>
struct executor_type {
    using result_type = void;

    template <typename F>
    void operator()(F f) const {
        using f_t = decltype(f);

        pp::Module::Get()->core()->CallOnMainThread(0,
                                                    pp::CompletionCallback(
                                                        [](void* f_, int32_t) {
                                                            auto f = static_cast<f_t*>(f_);
                                                            (*f)();
                                                            delete f;
                                                        },
                                                        new f_t(std::move(f))),
                                                    0);
    }
};

#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_WINDOWS

constexpr auto platform_priority(executor_priority p)
{
    switch (p)
    {
        case executor_priority::low:
            return TP_CALLBACK_PRIORITY_LOW;
        case executor_priority::medium:
            return TP_CALLBACK_PRIORITY_NORMAL;
        case executor_priority::high:
            return TP_CALLBACK_PRIORITY_HIGH;
    }
}

template <executor_priority P = executor_priority::medium>
class task_system {
    PTP_POOL _pool = nullptr;
    TP_CALLBACK_ENVIRON _callBackEnvironment;
    PTP_CLEANUP_GROUP _cleanupgroup = nullptr;

public:
    task_system() {
        InitializeThreadpoolEnvironment(&_callBackEnvironment);
        _pool = CreateThreadpool(nullptr);
        if (_pool == nullptr) throw std::bad_alloc();

        _cleanupgroup = CreateThreadpoolCleanupGroup();
        if (_cleanupgroup == nullptr) {
            CloseThreadpool(_pool);
            throw std::bad_alloc();
        }

        SetThreadpoolCallbackPriority(&_callBackEnvironment, platform_priority(P));
        SetThreadpoolCallbackPool(&_callBackEnvironment, _pool);
        SetThreadpoolCallbackCleanupGroup(&_callBackEnvironment, _cleanupgroup, nullptr);
    }

    ~task_system() {
        CloseThreadpoolCleanupGroupMembers(_cleanupgroup, FALSE, nullptr);
        CloseThreadpoolCleanupGroup(_cleanupgroup);
        CloseThreadpool(_pool);
    }

    template <typename F>
    void operator()(F&& f) {
        auto work = CreateThreadpoolWork(&callback_impl<F>, new F(std::forward<F>(f)),
                                         &_callBackEnvironment);

        if (work == nullptr) {
            throw std::bad_alloc();
        }
        SubmitThreadpoolWork(work);
    }

private:
    template <typename F>
    static void CALLBACK callback_impl(PTP_CALLBACK_INSTANCE /*instance*/,
                                       PVOID parameter,
                                       PTP_WORK /*Work*/) {
        std::unique_ptr<F> f(static_cast<F*>(parameter));
        (*f)();
    }
};

/**************************************************************************************************/
/**************************************************************************************************/

#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_PORTABLE


class notification_queue {
    using lock_t = std::unique_lock<std::mutex>;
    std::deque<task<void()>> _q;
    bool _done{false};
    std::mutex* _mutex{nullptr};
    std::condition_variable* _ready{nullptr};

public:
    void set_context(std::mutex& m, std::condition_variable& cv) {
        _mutex = &m;
        _ready = &cv;
    }

    bool try_pop(task<void()>& x) {
        lock_t lock{*_mutex, std::try_to_lock};
        if (!lock || _q.empty()) return false;
        x = std::move(_q.front());
        _q.pop_front();
        return true;
    }

    template <typename F>
    bool try_push(F&& f) {
        {
            lock_t lock{*_mutex, std::try_to_lock};
            if (!lock) return false;
            _q.emplace_back(std::forward<F>(f));
        }
        _ready->notify_all();
        return true;
    }

    void done() {
        {
            lock_t lock{*_mutex};
            _done = true;
        }
        _ready->notify_all();
    }

    bool pop(task<void()>& x) {
        lock_t lock{*_mutex};
        while (_q.empty() && !_done)
            _ready->wait(lock);
        if (_q.empty()) return false;
        x = std::move(_q.front());
        _q.pop_front();
        return true;
    }

    template <typename F>
    void push(F&& f) {
        {
            lock_t lock{*_mutex};
            _q.emplace_back(std::forward<F>(f));
        }
        _ready->notify_all();
    }
};

/**************************************************************************************************/

class priority_task_system {
    using lock_t = std::unique_lock<std::mutex>;

    const unsigned _count{std::thread::hardware_concurrency()};

    struct thread_context
    {
        std::thread thread;
        std::mutex mutex;
        std::condition_variable ready;
    };
    std::vector<thread_context> _threads{_count};
    std::array<std::vector<notification_queue>, 3> _q;
    std::atomic<unsigned> _index{0};
    bool _done{false};

    void run(unsigned i) {
        while (true) {
            task<void()> f;

            for (unsigned q = 0; q < 3; ++q) {
                for (unsigned n = 0; n != _count * 32; ++n) {
                    if (_q[q][(i + n) % _count].try_pop(f)) break;
                }
                if (f) break;
            }

            if (!f) {
                lock_t lock{_threads[i].mutex};
                while (_q[0].empty() && _q[1].empty() && _q[2].empty() && !_done)
                    _threads[i].ready.wait(lock);
            }
            else
                f();
        }
    }

public:
    priority_task_system() {
        for (auto q = 0; q < 3; ++q) {
            std::vector<notification_queue> resized{_count};
            std::swap(_q[q], resized);
            for (unsigned n = 0; n != _count; ++n) {
                _q[q][n].set_context(_threads[n].mutex, _threads[n].ready);
            }
        }
        for (unsigned n = 0; n != _count; ++n) {
            _threads[n].thread = std::thread([&, n] { run(n); });
        }
    }

    ~priority_task_system() {
        _done = true;
        for (auto q = 0; q < 3; ++q)
            for (auto& e : _q[q])
                e.done();

        for (auto& e : _threads)
            e.thread.join();
    }

    template <typename F>
    void high(F&& f) {
        auto i = _index++;

        for (unsigned n = 0; n != _count; ++n) {
            if (_q[0][(i + n) % _count].try_push(std::forward<F>(f))) return;
        }

        _q[0][i % _count].push(std::forward<F>(f));
    }

    template <typename F>
    void medium(F&& f) {
        auto i = _index++;

        for (unsigned n = 0; n != _count; ++n) {
            if (_q[1][(i + n) % _count].try_push(std::forward<F>(f))) return;
        }

        _q[1][i % _count].push(std::forward<F>(f));
    }

    template <typename F>
    void low(F&& f) {
        auto i = _index++;

        for (unsigned n = 0; n != _count; ++n) {
            if (_q[2][(i + n) % _count].try_push(std::forward<F>(f))) return;
        }

        _q[2][i % _count].push(std::forward<F>(f));
    }
};

inline priority_task_system& pts() {
    static priority_task_system only_task_system;
    return only_task_system;
}

template <executor_priority P = executor_priority::medium>
struct task_system
{
    using result_type = void;

    void operator()(task<void()> f) const {
        if constexpr (P == executor_priority::high)
        {
            pts().high(std::move(f));
        }
        else if constexpr (P == executor_priority::medium)
        {
            pts().medium(std::move(f));
        }
        else if constexpr (P == executor_priority::low)
        {
            pts().low(std::move(f));
        }
    }
};


#endif

#if (STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_PORTABLE) || \
    (STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_WINDOWS)

template <executor_priority P = executor_priority::medium>
struct executor_type {
    using result_type = void;

    void operator()(task<void()> f) const {
        static task_system<P> only_task_system;
        only_task_system(std::move(f));
    }
};

#endif

/**************************************************************************************************/

} // namespace detail

/**************************************************************************************************/

constexpr auto low_executor = detail::executor_type<detail::executor_priority::low>{};
constexpr auto default_executor = detail::executor_type<detail::executor_priority::medium>{};
constexpr auto high_executor = detail::executor_type<detail::executor_priority::high>{};

/**************************************************************************************************/

} // namespace v1

/**************************************************************************************************/

} // namespace stlab

/**************************************************************************************************/

#endif // STLAB_CONCURRENCY_DEFAULT_EXECUTOR_HPP
