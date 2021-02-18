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

#include <cassert>
#include <chrono>
#include <functional>

#if STLAB_TASK_SYSTEM(LIBDISPATCH)
#include <dispatch/dispatch.h>
#elif STLAB_TASK_SYSTEM(EMSCRIPTEN)
#include <emscripten.h>
#elif STLAB_TASK_SYSTEM(PNACL)
#include <ppapi/cpp/completion_callback.h>
#include <ppapi/cpp/core.h>
#include <ppapi/cpp/module.h>
#elif STLAB_TASK_SYSTEM(WINDOWS)
#include <Windows.h>
#include <memory>
#elif STLAB_TASK_SYSTEM(PORTABLE)

#include <algorithm>
#include <array>
#include <atomic>
#include <condition_variable>
#include <thread>
#include <vector>

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

#if STLAB_TASK_SYSTEM(LIBDISPATCH)

constexpr auto platform_priority(executor_priority p)
{
    switch (p)
    {
        case executor_priority::high:
            return DISPATCH_QUEUE_PRIORITY_HIGH;
        case executor_priority::medium:
          return DISPATCH_QUEUE_PRIORITY_DEFAULT;
        case executor_priority::low:
            return DISPATCH_QUEUE_PRIORITY_LOW;
        default:
            assert(!"Unknown value!");
    }
    return DISPATCH_QUEUE_PRIORITY_DEFAULT;
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

/**************************************************************************************************/

#elif STLAB_TASK_SYSTEM(EMSCRIPTEN)

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

/**************************************************************************************************/

#elif STLAB_TASK_SYSTEM(PNACL)

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

/**************************************************************************************************/

#elif STLAB_TASK_SYSTEM(WINDOWS)

constexpr auto platform_priority(executor_priority p)
{
    switch (p)
    {
        case executor_priority::high:
            return TP_CALLBACK_PRIORITY_HIGH;
        case executor_priority::medium:
            return TP_CALLBACK_PRIORITY_NORMAL;
        case executor_priority::low:
            return TP_CALLBACK_PRIORITY_LOW;
        default:
            assert(!"Unknown value!");

    }
    return TP_CALLBACK_PRIORITY_NORMAL;
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
                                       PTP_WORK work) {
        std::unique_ptr<F> f(static_cast<F*>(parameter));
        (*f)();
        CloseThreadpoolWork(work);
    }
};

/**************************************************************************************************/

#elif STLAB_TASK_SYSTEM(PORTABLE)

inline auto queue_size() {
#ifdef STLAB_UNIT_TEST
    // The test cannot run with less than two cores
    return std::max(2u, std::thread::hardware_concurrency());
#else
    return std::max(1u, std::thread::hardware_concurrency());
#endif
}

class notification_queue {
    using lock_t = std::unique_lock<std::mutex>;

    struct element_t {
        unsigned _priority;
        task<void()> _task;

        template <class F>
        element_t(F&& f, unsigned priority) : _priority{priority}, _task{std::forward<F>(f)} { }

        struct greater {
            bool operator()(const element_t& a, const element_t& b) const {
                return b._priority < a._priority;
            }
        };
    };

    std::vector<element_t> _q; // can't use priority queue because top() is const
    bool _done{false};
    std::mutex _mutex;
    std::condition_variable _ready;

    // This must be called under a lock with a non-empty _q
    task<void()> pop_not_empty() {
        auto result = std::move(_q.front()._task);
        std::pop_heap(begin(_q), end(_q), element_t::greater());
        _q.pop_back();
        return result;
    }

public:
    bool try_pop(task<void()>& x) {
        lock_t lock{_mutex, std::try_to_lock};
        if (!lock || _q.empty()) return false;
        x = pop_not_empty();
        return true;
    }

    bool pop(task<void()>& x) {
        lock_t lock{_mutex};
        while (_q.empty() && !_done) _ready.wait(lock);
        if (_q.empty()) return false;
        x = pop_not_empty();
        return true;
    }

    void done() {
        {
            lock_t lock{_mutex};
            _done = true;
        }
        _ready.notify_all();
    }

    template <typename F>
    bool try_push(F&& f, unsigned priority) {
        {
            lock_t lock{_mutex, std::try_to_lock};
            if (!lock) return false;
            _q.emplace_back(std::forward<F>(f), priority);
            std::push_heap(begin(_q), end(_q), element_t::greater());
        }
        _ready.notify_one();
        return true;
    }

    template <typename F>
    void push(F&& f, unsigned priority) {
        {
            lock_t lock{_mutex};
            _q.emplace_back(std::forward<F>(f), priority);
            std::push_heap(begin(_q), end(_q), element_t::greater());
        }
        _ready.notify_one();
    }
};

/**************************************************************************************************/

class priority_task_system {
    using lock_t = std::unique_lock<std::mutex>;

    const unsigned _count{queue_size()};

    std::vector<std::thread> _threads;
    std::vector<notification_queue> _q{_count};
    std::atomic<unsigned> _index{0};
    std::atomic_bool _done{false};

    void run(unsigned i) {
        while (true) {
            task<void()> f;

            for (unsigned n = 0; n != _count; ++n) {
                if (_q[(i + n) % _count].try_pop(f)) break;
            }
            if (!f && !_q[i].pop(f)) break;

            f();
        }
    }

public:
    priority_task_system() {
        _threads.reserve(_count);
        for (unsigned n = 0; n != _count; ++n) {
            _threads.emplace_back([&, n]{ run(n); });
        }
    }

    ~priority_task_system() {
        for (auto& e : _q) e.done();
        for (auto& e : _threads) e.join();
    }



    template <std::size_t P, typename F>
    void execute(F&& f) {
        static_assert(P < 3, "More than 3 priorities are not known!");
        auto i = _index++;

        for (unsigned n = 0; n != _count; ++n) {
            if (_q[(i + n) % _count].try_push(std::forward<F>(f), P)) return;
        }

        _q[i % _count].push(std::forward<F>(f), P);
    }

    bool steal() {
        task<void()> f;

        for (unsigned n = 0; n != _count; ++n) {
            if (_q[n].try_pop(f)) break;
        }
        if (!f) return false;

        f();

        return true;
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
        pts().execute<static_cast<std::size_t>(P)>(std::move(f));
    }
};

#endif

/**************************************************************************************************/

#if STLAB_TASK_SYSTEM(WINDOWS) || STLAB_TASK_SYSTEM(PORTABLE)

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

/**************************************************************************************************/
