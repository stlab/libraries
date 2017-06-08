/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_SERIAL_QUEUE_HPP
#define STLAB_SERIAL_QUEUE_HPP

/**************************************************************************************************/

#include <atomic>
#include <deque>
#include <utility>
#include <mutex>
#include <tuple>

#include <stlab/concurrency/future.hpp>
#include <stlab/concurrency/task.hpp>

/**************************************************************************************************/

namespace stlab {

/**************************************************************************************************/

inline namespace v1 {

/**************************************************************************************************/

namespace detail {

/**************************************************************************************************/

using executor_t = std::function<void (task&&)>;

/**************************************************************************************************/

class serial_instance_t : public std::enable_shared_from_this<serial_instance_t> {
    executor_t       _executor;
    std::deque<task> _queue;
    std::mutex       _m;
    bool             _running{false};

    void dequeue() {
        task f;

        {
        std::lock_guard<std::mutex> lock(_m);

        std::swap(f, _queue.front());

        _queue.pop_front();
        }

        f();
    }

    void execute_dequeue() {
        _executor([_this = shared_from_this()](){ _this->dequeue(); });
    }

    void try_dequeue() {
        std::lock_guard<std::mutex> lock(_m);

        if (_queue.empty()) {
            _running = false;
            return;
        }

        execute_dequeue();
    }

public:
    template <typename F>
    void enqueue(F&& f) {
        bool running(true);

        {
            std::lock_guard<std::mutex> lock(_m);

            _queue.emplace_back([_f = std::forward<F>(f), _impl(shared_from_this())](){
                _f();
                _impl->try_dequeue();
            });

            // A trick to get the value of _running within the lock scope, but then
            // use it outside the scope, after the lock has been released. It also
            // sets running to true if it is not yet; two birds, one stone.
            std::swap(running, _running);
        }

        if (!running) {
            execute_dequeue();
        }
    }

    serial_instance_t(detail::executor_t&& executor) : _executor(std::move(executor)) {
    }
};

/**************************************************************************************************/

} // namespace detail

/**************************************************************************************************/

class serial_queue_t {
    std::shared_ptr<detail::serial_instance_t> _impl;

public:
    template <typename Executor>
    serial_queue_t(Executor e) : _impl(std::make_shared<detail::serial_instance_t>([_e = e](auto&& f){
        _e(std::move(f));
    })) { }

    auto executor() const {
        return [_impl = _impl](auto f) {
            _impl->enqueue(std::move(f));
        };
    }

    template <typename F, typename... Args>
    auto operator()(F&& f, Args&&... args) {
        return async(executor(), std::forward<F>(f), std::forward<Args>(args)...);
    }
};

/**************************************************************************************************/

} // namespace v1
} // namespace stlab

/**************************************************************************************************/

#endif

/**************************************************************************************************/
