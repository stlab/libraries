/*
    Copyright 2023 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#pragma once
#ifndef STLAB_CONCURRENCY_ACTOR_HPP
#define STLAB_CONCURRENCY_ACTOR_HPP

// stdc++
#include <memory>
#include <optional>

// stlab
#include <stlab/concurrency/serial_queue.hpp>
#include <stlab/concurrency/set_current_thread_name.hpp>

// lambda pack captures are a C++20 feature, but both clang and msvc support it, so we should be
// okay to use it even under C++17.
#if __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++20-extensions"
#endif // __clang__

/**************************************************************************************************/

namespace stlab {
inline namespace v1 {

/**************************************************************************************************/

namespace detail {

/**************************************************************************************************/

struct temp_thread_name {
    explicit temp_thread_name(const char* name) { stlab::set_current_thread_name(name); }
    ~temp_thread_name() { stlab::set_current_thread_name(""); }
};

/**************************************************************************************************/

template <class T>
struct value_instance {
    std::optional<T> _x;
};

template <>
struct value_instance<void> {};

/**************************************************************************************************/

template <class T>
struct actor_instance : public std::enable_shared_from_this<actor_instance<T>> {
    template <class Executor>
    explicit actor_instance(Executor&& e, std::string&& name) :
        _q(std::forward<Executor>(e)), _name(std::move(name)) {}

    template <class... Args>
    void initialize(Args&&... args) {
        // We want to construct the object instance in the executor where
        // it will be running. We cannot initialize in the constructor because
        // `shared_from_this` will throw `bad_weak_ptr`.
        _q(
            [_this = this->shared_from_this()](auto&&... args) mutable {
                temp_thread_name ttn(_this->_name.c_str());
                _this->_instance._x = T(std::forward<Args>(args)...);
            },
            std::forward<Args>(args)...)
            .detach();
    }

    auto set_name(std::string&& name) { _name = std::move(name); }

    template <typename F, typename... Args>
    auto send(F&& f, Args&&... args) {
        auto task = [_f = std::forward<F>(f),
                     _this = this->shared_from_this()](auto&&... args) mutable {
            temp_thread_name ttn(_this->_name.c_str());
            if constexpr (std::is_same_v<T, void>) {
                return std::move(_f)(std::forward<Args>(args)...);
            } else {
                return std::move(_f)(*(_this->_instance._x), std::forward<Args>(args)...);
            }
        };
        return _q(std::move(task), std::forward<Args>(args)...);
    }

    template <typename R, typename F, typename... Args>
    auto then(stlab::future<R>&& future, F&& f, Args&&... args) {
        auto task = [_f = std::forward<F>(f), ... _args = std::forward<Args>(args),
                     _this = this->shared_from_this()](R&& x) mutable {
            temp_thread_name ttn(_this->_name.c_str());
            if constexpr (std::is_same_v<T, void>) {
                return std::move(_f)(std::forward<R>(x), std::move(_args)...);
            } else {
                return std::move(_f)(*(_this->_instance._x), std::forward<R>(x),
                                     std::move(_args)...);
            }
        };
        return std::move(future).then(_q.executor(), std::move(task));
    }

    template <typename F, typename... Args>
    auto then(stlab::future<void>&& future, F&& f, Args&&... args) {
        auto task = [_f = std::forward<F>(f), ... _args = std::forward<Args>(args),
                     _this = this->shared_from_this()]() mutable {
            temp_thread_name ttn(_this->_name.c_str());
            if constexpr (std::is_same_v<T, void>) {
                return std::move(_f)(std::move(_args)...);
            } else {
                return std::move(_f)(*(_this->_instance._x), std::move(_args)...);
            }
        };
        return std::move(future).then(_q.executor(), std::move(task));
    }

    value_instance<T> _instance;
    stlab::serial_queue_t _q;
    std::string _name;
};

/**************************************************************************************************/

} // namespace detail

/**************************************************************************************************/

template <class T>
class actor {
    std::shared_ptr<detail::actor_instance<T>> _impl;

public:
    actor() = default;

    template <class Executor, class... Args>
    actor(Executor&& e, std::string&& name, Args&&... args) :
        _impl(std::make_shared<detail::actor_instance<T>>(std::forward<Executor>(e),
                                                          std::move(name))) {
        if constexpr (!std::is_same_v<T, void>) {
            _impl->initialize(std::forward<Args>(args)...);
        }
    }

    auto set_name(std::string&& name) { _impl->set_name(std::move(name)); }

    template <typename F, typename... Args>
    auto send(F&& f, Args&&... args) {
        return _impl->send(std::forward<F>(f), std::forward<Args>(args)...);
    }

    template <typename R, typename F, typename... Args>
    auto then(stlab::future<R>&& future, F&& f, Args&&... args) {
        return _impl->then(std::move(future), std::forward<F>(f), std::forward<Args>(args)...);
    }

    friend bool operator==(const actor& x, const actor& y) { return x._impl == y._impl; }
    friend bool operator!=(const actor& x, const actor& y) { return !(x == y); }
};

/**************************************************************************************************/

} // namespace v1
} // namespace stlab

/**************************************************************************************************/

#if __clang__
#pragma clang diagnostic pop
#endif // __clang__

/**************************************************************************************************/

#endif // STLAB_CONCURRENCY_ACTOR_HPP

/**************************************************************************************************/
