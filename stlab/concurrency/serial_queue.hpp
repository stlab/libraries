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

enum class empty_mode {
    dequeue,
    drain
};

/**************************************************************************************************/

namespace detail {

/**************************************************************************************************/

class serial_instance_t : public std::enable_shared_from_this<serial_instance_t> {
    using executor_t = std::function<void (task&&)>;
    using queue_t = std::deque<task>;
    using lock_t = std::lock_guard<std::mutex>;

    std::mutex       _m;
    bool             _running{false}; // mutex protects this
    queue_t          _queue;          // mutex protects this
    executor_t       _executor;
    void (serial_instance_t::*_kickstart)();

    static auto pop_front_unsafe(queue_t& q) {
        auto f = std::move(q.front());
        q.pop_front();
        return f;
    }

    bool empty() {
        bool empty;

        {
        lock_t lock(_m);
        
        empty = _queue.empty();

        if (empty) {
            _running = false;
        }
        }

        return empty;
    }

    void drain() {
        queue_t local_queue;

        do {
            {
            lock_t lock(_m);

            std::swap(local_queue, _queue);
            }

            while (!local_queue.empty()) {
                pop_front_unsafe(local_queue)();
            }
        } while (!empty());
    }

    void dequeue() {
        task f;

        {
        lock_t lock(_m);

        f = pop_front_unsafe(_queue);
        }

        f();

        if (!empty())
            _executor([_this(shared_from_this())](){ _this->dequeue(); });
    }

    // The kickstart allows us to grab a pointer to either the dequeue or drain
    // routine at construction time. When it comes time to process the queue, we
    // call either via the abstracted _kickstart.
    void kickstart() {
        (this->*_kickstart)();
    }

    static auto kickstarter(empty_mode mode) {
        switch (mode) {
            case empty_mode::dequeue: return &serial_instance_t::dequeue;
            case empty_mode::drain:   return &serial_instance_t::drain;
        }
    }

public:
    template <typename F>
    void enqueue(F&& f) {
        bool running(true);

        {
        lock_t lock(_m);

        _queue.emplace_back(std::forward<F>(f));

        // A trick to get the value of _running within the lock scope, but then
        // use it outside the scope, after the lock has been released. It also
        // sets running to true if it is not yet; two birds, one stone.
        std::swap(running, _running);
        }

        if (!running) {
            _executor([_this(shared_from_this())](){ _this->kickstart(); });
        }
    }

    serial_instance_t(executor_t&& executor, empty_mode mode) :
        _executor(std::move(executor)),
        _kickstart(kickstarter(mode)) {
    }
};

/**************************************************************************************************/

} // namespace detail

/**************************************************************************************************/

class serial_queue_t {
    std::shared_ptr<detail::serial_instance_t> _impl;

public:
    template <typename Executor>
    explicit serial_queue_t(Executor e, empty_mode mode = empty_mode::dequeue) :
        _impl(std::make_shared<detail::serial_instance_t>([_e = e](auto&& f){
            _e(std::move(f));
        }, mode)) { }

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

/**************************************************************************************************/

} // namespace stlab

/**************************************************************************************************/

#endif

/**************************************************************************************************/
