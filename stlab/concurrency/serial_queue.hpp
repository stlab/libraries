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

#include <stlab/concurrency/future.hpp>

/**************************************************************************************************/

namespace stlab {

/**************************************************************************************************/

inline namespace v1 {

/**************************************************************************************************/

namespace detail {

/**************************************************************************************************/

class serial_instance_t {
    typedef std::function<void ()> work_t;

    std::function<void (work_t&&)> _executor;
    std::deque<work_t>             _queue;
    std::mutex                     _m;
    bool                           _running{false};

    struct set_bool_t {
        set_bool_t(bool& b) noexcept : _b(b) { _b = true; }
        ~set_bool_t() noexcept { _b = false; }
        bool& _b;
    };

    void dequeue(std::unique_lock<std::mutex>& lock) {
        set_bool_t set_running(_running);
        auto       f(std::move(_queue.front()));

        _queue.pop_front();

        lock.unlock();

        f();
    }

    void loop() {
        std::unique_lock<std::mutex> queue_lock(_m);

        if (_running || _queue.empty()) {
            return;
        }

        run(queue_lock);

        post_loop();
    }

    void post_loop() {
        _executor(std::bind(&serial_instance_t::loop, this));
    }

public:
    template <typename F, typename... Args>
    auto enqueue(F&& f, Args&&... args) {
        using result_t = decltype(std::declval<F>()(std::declval<Args>()...));
        auto p = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        auto pt = stlab::package<result_t()>(_executor, p);

        {
        std::lock_guard<std::mutex> lock(_m);

        _queue.emplace_back(std::move(pt.first));
        }

        post_loop();

        pt.second.detach();

        return pt.second;
    }

    serial_instance_t(std::function<void (work_t&&)> executor) : _executor(executor) {
    }

    ~serial_instance_t() {
        while (true) {
            std::unique_lock<std::mutex> queue_lock(_m);

            if (_running)
                continue;

            if (_queue.empty())
                break;

            run(queue_lock);
        }
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
            _impl->enqueue(std::forward<decltype(f)>(f));
        };
    }
};

/**************************************************************************************************/

} // namespace v1
} // namespace stlab

/**************************************************************************************************/

#endif

/**************************************************************************************************/
