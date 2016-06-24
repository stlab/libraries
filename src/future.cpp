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
#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_WINDOWS

#include <Windows.h>

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

    // TODO FP: delayed execution to be implemented
    template <typename F>
    void async_(std::chrono::system_clock::time_point, F&& f) {
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

FILETIME time_point_to_FILETIME(const std::chrono::system_clock::time_point& tp) {
    FILETIME ft = { 0, 0 };
    SYSTEMTIME st = { 0 };
    time_t t = std::chrono::system_clock::to_time_t(tp);
    tm utc_tm;
    if (!gmtime_s(&utc_tm, &t)) {
        st.wSecond = static_cast<WORD>(utc_tm.tm_sec);
        st.wMinute = static_cast<WORD>(utc_tm.tm_min);
        st.wHour = static_cast<WORD>(utc_tm.tm_hour);
        st.wDay = static_cast<WORD>(utc_tm.tm_mday);
        st.wMonth = static_cast<WORD>(utc_tm.tm_mon + 1);
        st.wYear = static_cast<WORD>(utc_tm.tm_year + 1900);
        st.wMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()).count() % 1000;
        SystemTimeToFileTime(&st, &ft);
    }
    return ft;
}


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
    void async_(std::chrono::system_clock::time_point time_point, F&& f) {

        auto timer = CreateThreadpoolTimer(&timer_callback_impl<F>,
                                           new F(std::forward<F>(f)),
                                           nullptr);
        if (timer == nullptr) {
            throw std::bad_alloc();
        }

        auto file_time = time_point_to_FILETIME(time_point);

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
                                             PTP_TIMER             /*timer*/ ) {
        std::unique_ptr<F> f(static_cast<F*>(parameter));
        (*f)();
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

task_system& only_task_system() {
    static task_system instance;
    return instance;
}

void async_(std::chrono::system_clock::time_point time_point, function<void()> f) {
    if (time_point > std::chrono::system_clock::now())
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
