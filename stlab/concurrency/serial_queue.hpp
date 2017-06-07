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
        std::unique_lock<std::mutex> lock(_m);

        if (_queue.empty()) {
            _running = false;
            return;
        }

        _running = true;

        auto f(std::move(_queue.front()));

        _queue.pop_front();

        lock.unlock();

        f();
    }

public:
    template <typename F, typename... Args>
    auto enqueue(F&& f, Args&&... args) {
        using result_t = decltype(std::declval<F>()(std::declval<Args>()...));
        auto p = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        auto pt = stlab::package<result_t()>(_executor, p);

        {
        std::unique_lock<std::mutex> lock(_m);

        _queue.emplace_back([_f(std::move(pt.first)), _impl(shared_from_this())](){
            _f();
            _impl->dequeue();
        });
        }

        if (!_running)
            dequeue();

        return pt.second;
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

    template <typename F, typename... Args>
    auto operator()(F&& f, Args&&... args) {
        return _impl->enqueue(std::forward<F>(f), std::forward<Args>(args)...);
    }

    auto executor() const {
        return [_impl = _impl](auto&& f) {
            (*_impl)(std::forward<decltype(f)>(f)).detach();
        };
    }
};

/**************************************************************************************************/

} // namespace v1
} // namespace stlab

/**************************************************************************************************/

#endif

/**************************************************************************************************/
