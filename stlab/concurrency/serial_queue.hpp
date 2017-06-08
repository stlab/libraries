/*
Copyright 2017 Adobe
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLABFUTURE_SERIAL_QUEUE
#define STLABFUTURE_SERIAL_QUEUE

#include "config.hpp"

#include <deque>
#include <functional>
#include <memory>
#include <mutex>
#include <atomic>

#if STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_LIBDISPATCH
#include <dispatch/dispatch.h>
#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_WINDOWS
//#include <Windows.h>
//#include <memory>
#include "default_executor.hpp"
#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_PORTABLE
#include "default_executor.hpp"
#endif

/**************************************************************************************************/

namespace stlab
{

/**************************************************************************************************/

inline namespace v1
{

/**************************************************************************************************/

namespace detail
{

#if STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_LIBDISPATCH

class shared_t
{
    dispatch_queue_t _q = dispatch_queue_create("com.mutexpp.serial_queue", nullptr);

public:
    shared_t() = default;

    shared_t(const shared_t &rhs) : _q(rhs._q) {
        dispatch_retain(_q);
    }

    shared_t(const char *name) : _q{dispatch_queue_create(name, nullptr)} {
    }

    ~shared_t() {
        dispatch_release(_q);
    }

    shared_t &operator=(const shared_t &rhs) {
        if (_q != rhs._q) {
            dispatch_release(_q);
            _q = rhs._q;
            dispatch_retain(_q);
        }

        return *this;
    }

    template<typename F>
    void enqueue(F f) const {
        using f_t = decltype(f);

        dispatch_async_f(_q,
                         new f_t(std::move(f)),
                         [](void *f_) {
                             auto f = static_cast<f_t *>(f_);
                             (*f)();
                             delete f;
                         });
    }
};

/**************************************************************************************************/

#else

/**************************************************************************************************/

template <typename Executor, typename Enabled = void>
struct queue_t;

template <typename Executor>
struct queue_t<Executor, typename std::enable_if_t<Executor::is_serial>> : std::enable_shared_from_this<queue_t<Executor>>
{
    Executor                                   _executor;
    std::deque<std::function<void()>>          _queue;
    std::atomic_bool                           _running{false};

    queue_t(Executor executor = Executor{})
            : _executor(std::move(executor)) {}

    template <typename F>
    void enqueue(F&& f) {
        _queue.emplace_back([_this = this->shared_from_this(), _f = std::forward<F>(f)]() {
            _f();
            _this->dequeue();
        });
        if (!_running)
            dequeue();
    }

    void dequeue() {
        if (_queue.empty()) {
            _running = false;
            return;
        }
        _running = true;
        auto f = std::move(_queue.front());
        _queue.pop_front();

        _executor(f);
    }
};

template <typename Executor>
struct queue_t<Executor, std::enable_if_t<!Executor::is_serial>> : std::enable_shared_from_this<queue_t<Executor>>
{
    using lock_t = std::unique_lock<std::mutex>;

    Executor                                   _executor;
    std::mutex                                 _mutex;
    std::deque<std::function<void()>>          _queue;
    std::atomic_bool                           _running{false};

    queue_t(Executor executor = Executor{})
        : _executor(std::move(executor)) {}

    template <typename F>
    void enqueue(F&& f) {
        {
            lock_t lock(_mutex);
            _queue.emplace_back([ _this = this->shared_from_this(), _f(std::forward<F>(f)) ]() {
                _f();
                _this->dequeue();
            });
        }
        if (!_running)
            dequeue();
    }

    void dequeue() {
        std::function<void()> f;
        {
            lock_t lock(_mutex);

            if (_queue.empty()) {
                _running = false;
                return;
            }
            _running = true;
            f = std::move(_queue.front());
            _queue.pop_front();
        }
        _executor(f);
    }
};

using shared_t = queue_t<stlab::default_executor_t>;


#endif



} // namespace detail





class serial_queue
{
    std::shared_ptr<detail::shared_t> _shared;

  public:
    template <typename... Args>
    serial_queue(Args... args)
        : _shared(std::make_shared<detail::shared_t>(std::forward<Args>(args)...)) {}

    serial_queue(const serial_queue&) = default;
    serial_queue& operator=(const serial_queue&) = default;
    serial_queue (serial_queue&& ) = default;
    serial_queue& operator=(serial_queue&&) = default;

    // return an executor to schedule tasks serially
    auto executor() const {
        return [_shared = _shared](auto f) {
            _shared->enqueue(std::move(f));
        };
    }
};


} // namespace v1

/**************************************************************************************************/

} // namespace stlab

/**************************************************************************************************/

#endif