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
#include <Windows.h>
#include <memory>
#elif STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_PORTABLE
#endif

/**************************************************************************************************/

namespace stlab
{

/**************************************************************************************************/

inline namespace v1
{

namespace detail
{
    
}

template <typename E>
class serial_queue
{
    using lock_t = std::unique_lock<std::mutex>;

    template <typename Executor, typename Enabled = void>
    struct shared_t;

    template <typename Executor>
    struct shared_t<Executor, std::enable_if_t<E::is_serial, E>> : std::enable_shared_from_this<shared_t<Executor>>
    {
        Executor                                   _executor;
        std::deque<std::function<void()>>          _queue;
        bool                                       _running = false;

        shared_t(Executor executor)
            : _executor(std::move(executor)) {}

        template <typename F>
        void enqueue(F&& f) {
            _queue.emplace_back([_this = shared_from_this(), _f(std::forward<F>(f))]() {
                _f();
                _this->dequeue();
            });
            if (!_running)
                dequeue();
        }

        void dequeue() {
            std::function<void()> f;
            if (_queue.empty()) {
                _running = false;
                return;
            }
            _running = true;
            f = std::move(_queue.front());
            _queue.pop_front();

            _executor(f);
        }
    };

    template <typename Executor>
    struct shared_t<Executor, std::enable_if_t<!Executor::is_serial, Executor>> : std::enable_shared_from_this<shared_t<Executor>>
    {
        Executor                                   _executor;
        std::mutex                                 _mutex;
        std::deque<std::function<void()>>          _queue;
        std::atomic_bool                           _running = false;

        shared_t(Executor executor)
            : _executor(std::move(executor)) {}

        template <typename F>
        void enqueue(F&& f) {
            {
                lock_t lock(_mutex);
                _queue.emplace_back([ _this = shared_from_this(), _f(std::forward<F>(f)) ]() {
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

    std::shared_ptr<shared_t<E>> _shared;

  public:

    serial_queue(E executor)
        : _shared(std::make_shared<shared_t<E>>(move(executor))) {}

    // return an executor to schedule tasks serially
    auto executor() const {
        return [_shared = _shared](auto&& f) {
            _shared->enqueue(std::forward<decltype(f)>(f));
        };
    }
};

} // namespace v1

/**************************************************************************************************/

} // namespace stlab

/**************************************************************************************************/

#endif