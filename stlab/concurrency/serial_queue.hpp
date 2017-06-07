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
// Task are fully captured functions with a mutable call operator to support moving items through.

/*
    REVISIT (sparent) : Add small object optimizations and reference wrapper support to match
    std::function. Support arguments?
*/
class task {
    struct concept {
        virtual ~concept() { }
        virtual void invoke() = 0;
        virtual const std::type_info& target_type() const = 0;
        virtual void* pointer() = 0;
        virtual const void* pointer() const = 0;
    };

    template <class F>
    struct model : concept {
        model(F&& f) : _f(std::move(f)) { }
        void invoke() override { _f(); }
        const std::type_info& target_type() const override { return typeid(F); }
        void* pointer() override { return &_f; }
        const void* pointer() const override { return &_f; }

        F _f;
    };

    std::unique_ptr<concept> _self;

public:
    using result_type = void;

    task() noexcept = default;
    task(std::nullptr_t) : task() { }
    task(const task&) = delete;
    task(task&&) noexcept = default;
    template <class F>
    task(F&& f) : _self(std::make_unique<model<F>>(std::forward<F>(f))) { }

    ~task() noexcept = default;

    task& operator=(const task&) = delete;
    task& operator=(task&&) noexcept = default;
    task& operator=(std::nullptr_t) noexcept { _self.reset(); return *this; }
    template <class F>
    task& operator=(F&& f) { return *this = task(f); }
    // REVISIT (sparent) : Support reference wrapper assignment

    void swap(task& x) { std::swap(_self, x._self); }

    explicit operator bool() const { return static_cast<bool>(_self); }

    const std::type_info& target_type() const
    { return _self ? _self->target_type() : typeid(void); }

    template <class T>
    T* target() { return (target_type() == typeid(T)) ? _self->pointer() : nullptr; }
    template <class T>
    const T* target() const { return (target_type() == typeid(T)) ? _self->pointer() : nullptr; }


    void operator()() { _self->invoke(); }
};

inline void swap(task& x, task& y) { return x.swap(y); }
inline bool operator==(const task& x, std::nullptr_t) { return !static_cast<bool>(x); }
inline bool operator==(std::nullptr_t, const task& x) { return !static_cast<bool>(x); }
inline bool operator!=(const task& x, std::nullptr_t) { return static_cast<bool>(x); }
inline bool operator!=(std::nullptr_t, const task& x) { return static_cast<bool>(x); }

/**************************************************************************************************/

namespace detail {

/**************************************************************************************************/

using executor_t = std::function<void (task&&)>;

/**************************************************************************************************/

class serial_instance_t {
    executor_t       _executor;
    std::deque<task> _queue;
    std::mutex       _m;
    bool             _running{false};

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

        dequeue(queue_lock);

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

    serial_instance_t(detail::executor_t&& executor) : _executor(std::move(executor)) {
    }

    ~serial_instance_t() {
        while (true) {
            std::unique_lock<std::mutex> queue_lock(_m);

            if (_running)
                continue;

            if (_queue.empty())
                break;

            dequeue(queue_lock);
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
