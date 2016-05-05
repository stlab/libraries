/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#include <stlab/future.hpp>
#include <functional>

#if STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_PORTABLE

#include <atomic>
#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <condition_variable>

#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_WINDOWS

#include <windows.h>
#include <Threadpoolapiset.h>

#endif

using namespace std;

/**************************************************************************************************/


namespace {

#if STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_PORTABLE

/**************************************************************************************************/

using lock_t = unique_lock<mutex>;

class notification_queue {
    deque<function<void()>> _q;
    bool                    _done{false};
    mutex                   _mutex;
    condition_variable      _ready;
    
public:
    bool try_pop(function<void()>& x) {
        lock_t lock{_mutex, try_to_lock};
        if (!lock || _q.empty()) return false;
        x = move(_q.front());
        _q.pop_front();
        return true;
    }
    
    template<typename F>
    bool try_push(F&& f) {
        {
            lock_t lock{_mutex, try_to_lock};
            if (!lock) return false;
            _q.emplace_back(forward<F>(f));
        }
        _ready.notify_one();
        return true;
    }
    
    void done() {
        {
            unique_lock<mutex> lock{_mutex};
            _done = true;
        }
        _ready.notify_all();
    }
    
    bool pop(function<void()>& x) {
        lock_t lock{_mutex};
        while (_q.empty() && !_done) _ready.wait(lock);
        if (_q.empty()) return false;
        x = move(_q.front());
        _q.pop_front();
        return true;
    }
    
    template<typename F>
    void push(F&& f) {
        {
            lock_t lock{_mutex};
            _q.emplace_back(forward<F>(f));
        }
        _ready.notify_one();
    }
};

/**************************************************************************************************/

class task_system {
    const unsigned              _count{thread::hardware_concurrency()};
    vector<thread>              _threads;
    vector<notification_queue>  _q{_count};
    atomic<unsigned>            _index{0};
    
    void run(unsigned i) {
        while (true) {
            function<void()> f;
            
            for (unsigned n = 0; n != _count * 32; ++n) {
                if (_q[(i + n) % _count].try_pop(f)) break;
            }
            if (!f && !_q[i].pop(f)) break;
            
            f();
        }
    }
    
  public:
    task_system() {
        for (unsigned n = 0; n != _count; ++n) {
            _threads.emplace_back([&, n]{ run(n); });
        }
    }
    
    ~task_system() {
        for (auto& e : _q) e.done();
        for (auto& e : _threads) e.join();
    }
    
    template <typename F>
    void async_(F&& f) {
        auto i = _index++;
        
        for (unsigned n = 0; n != _count; ++n) {
            if (_q[(i + n) % _count].try_push(forward<F>(f))) return;
        }
        
        _q[i % _count].push(forward<F>(f));
    }
};

/**************************************************************************************************/

#if 0
struct timed_queue {
    using lock_t = unique_lock<mutex>;
    using element_t = pair<steady_clock::time_point, any_packaged_task_>;
    using queue_t = vector<element_t>;

    queue_t             _q;
    condition_variable  _condition;
    bool                _done = false;
    mutex               _mutex;
    thread              _thread;

    timed_queue() { _thread = thread([this]{ this->_run(); }); }

    ~timed_queue() {
        {
        lock_t lock(_mutex);
        _done = true;
        }
        _thread.join();
    }

    void _run()
    {
        while(true) {
            any_packaged_task_ task;
            {
            lock_t lock(_mutex);

            while (_q.empty() && !_done) _condition.wait(lock);
            if (_q.empty()) break; // ==> _done
            while (steady_clock::now() < _q.front().first) {
                auto t = _q.front().first;
                _condition.wait_until(lock, t);
            }
            pop_heap(begin(_q), end(_q), [](const auto& x, const auto& y) {
                return x.first > y.first;
            });
            task = move(_q.back().second);
            _q.pop_back();
            }

            adobe::details::async_(move(task));
        }
    }

    void async(const steady_clock::time_point& when, any_packaged_task_&& p)
    {
        {
        lock_t lock(_mutex);
        _q.push_back(element_t(when, move(p)));
        push_heap(begin(_q), end(_q), greater_first());
        }
        _condition.notify_one();
    }

};
#endif

/**************************************************************************************************/

#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_WINDOWS

class task_system
{
    PTP_POOL _thread_pool;
    TP_CALLBACK_ENVIRON _callback_environment;
public:
    task_system() {
        InitializeThreadpoolEnvironment(&_callback_environment);
        _thread_pool = CreateThreadpool(NULL);

        if (NULL == _thread_pool) {
            // Throw???
        }
        SetThreadpoolCallbackPool(&_callback_environment, _thread_pool);
    }

    ~task_system()
    {
        CloseThreadpool(_thread_pool);
    }

    template <typename F>
    static void CALLBACK callback_impl(PTP_CALLBACK_INSTANCE instance,
        PVOID                 parameter,
        PTP_WORK              Work) {
        auto f = static_cast<F*>(parameter);
        (*f)();
        delete f;
    }

    template <typename F>
    void async_(F&& f) {
        using f_t = decltype(f);
        
        auto work = CreateThreadpoolWork((&callback_impl<F>), new F(std::move(f)), &_callback_environment);

        if (NULL == work) {
            // todo throw?
        }

        SubmitThreadpoolWork(work);
    }
};

#endif

} // namespace

/**************************************************************************************************/

namespace stlab {
namespace detail {

/*
    REVISIT (sparent) : All of the templated code above for perfect forwarding doesn't do any
    good since we need a concrete interface at this level. Above code could be simpler.
*/

void async_(function<void()> f) {
    static task_system _system;
    _system.async_(move(f));
}

} // namespace detail
} // namespace stlab

/**************************************************************************************************/
