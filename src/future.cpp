/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#include <stlab/config.hpp>

#if (STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_PORTABLE) \
    || (STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_WINDOWS)

#include <stlab/future.hpp>
#include <functional>

#if STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_PORTABLE

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#endif

using namespace std;

/**************************************************************************************************/


namespace {

#if STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_PORTABLE

/**************************************************************************************************/

using lock_t = unique_lock<mutex>;

class notification_queue 
{
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

struct greater_first
{
    using result_type = bool;

    template <typename T>
    bool operator()(const T& x, const T& y) {
        return x.first > y.first;
    }
};

class task_system 
{
    const unsigned              _count{thread::hardware_concurrency()};
    vector<thread>              _threads;
    vector<notification_queue>  _q{_count};
    atomic<unsigned>            _index{0};

    using element_t = pair<chrono::system_clock::time_point, function<void()>>;
    using queue_t = vector<element_t>;

    queue_t             _timed_queue;
    condition_variable  _condition;
    bool                _stop = false;
    mutex               _timed_queue_mutex;
    thread              _timed_queue_thread;

    
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

    void timed_queue_run() {
        while (true) {
            function<void()> task;
            {
                lock_t lock(_timed_queue_mutex);

                while (_timed_queue.empty() && !_stop) _condition.wait(lock);
                if (_stop) break;
                while (chrono::system_clock::now() < _timed_queue.front().first) {
                    auto when = _timed_queue.front().first;
                    _condition.wait_until(lock, when);
                }
                pop_heap(begin(_timed_queue), end(_timed_queue), greater_first());
                task = move(_timed_queue.back().second);
                _timed_queue.pop_back();
            }

            async_(move(task));
        }
    }

public:
    task_system() {
        for (unsigned n = 0; n != _count; ++n) {
            _threads.emplace_back([&, n]{ run(n); });
        }
        _timed_queue_thread = thread([this] { this->timed_queue_run(); });
    }
    
    ~task_system() {
        {
            lock_t lock(_timed_queue_mutex);
            _stop = true;
        }
        _condition.notify_one();
        _timed_queue_thread.join();

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

    template <typename F>
    void async_(chrono::system_clock::time_point when, F&& f) {
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

/**************************************************************************************************/
#endif

} // namespace

/**************************************************************************************************/

namespace stlab {
namespace detail {

/*
    REVISIT (sparent) : All of the templated code above for perfect forwarding doesn't do any
    good since we need a concrete interface at this level. Above code could be simpler.
*/

task_system& only_task_system() {
    static task_system instance;
    return instance;
}

void async_(std::chrono::system_clock::time_point time_point, function<void()> f) {
    if ( (time_point != std::chrono::system_clock::time_point()) && (time_point > std::chrono::system_clock::now()) )        
        only_task_system().async_(time_point, move(f));
    else
        async_(std::move(f));
}

void async_(function<void()> f) {
    only_task_system().async_(move(f));
}

} // namespace detail
} // namespace stlab

/**************************************************************************************************/

#endif

/**************************************************************************************************/
