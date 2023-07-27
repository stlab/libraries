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

//------------------------------------------------------------------------------------------------------------------------------------------

namespace stlab {
inline namespace v1 {

//------------------------------------------------------------------------------------------------------------------------------------------

/// @brief Opaque type representing an identifier unique to an actor.
/// @hyde-owner fbrereto
enum class actor_id : std::intptr_t;

/// @brief Get the `actor_id` of the currently running actor.
/// @hyde-owner fbrereto
actor_id this_actor_id();

//------------------------------------------------------------------------------------------------------------------------------------------

/// @brief Support class to get details about the currently running actor.
/// @hyde-owner fbrereto
struct this_actor {
    /// @brief Get the `actor_id` of the currently running actor.
    /// @return The appropriate `actor_id`, or `0` if this is called outside the context of an actor.
    static actor_id get_id();

    /// @brief Get a reference to the currently running actor.
    /// @throw `std::runtime_error` if this is called outside the context of an actor.
    template <typename T>
    static decltype(auto) get();
};

//------------------------------------------------------------------------------------------------------------------------------------------

namespace detail {

//------------------------------------------------------------------------------------------------------------------------------------------

struct temp_thread_name {
    explicit temp_thread_name(const char* name) { stlab::set_current_thread_name(name); }
    ~temp_thread_name() { stlab::set_current_thread_name(""); }
};

//------------------------------------------------------------------------------------------------------------------------------------------

template <class T>
struct value_instance {
    std::optional<T> _x;
};

template <>
struct value_instance<void> {};

//------------------------------------------------------------------------------------------------------------------------------------------

struct actor_instance_base {
    virtual ~actor_instance_base() = default;

    actor_id id() const {
        return static_cast<actor_id>(reinterpret_cast<std::underlying_type_t<actor_id>>(this));
    }
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline actor_instance_base*& this_actor_accessor() {
    static thread_local actor_instance_base* this_actor{nullptr};
    return this_actor;
}

struct temp_this_actor {
    temp_this_actor(actor_instance_base* actor) {
        assert(actor);
        this_actor_accessor() = actor;
    }

    ~temp_this_actor() { this_actor_accessor() = nullptr; }
};

//------------------------------------------------------------------------------------------------------------------------------------------

template <class T>
struct actor_instance
    : public actor_instance_base
    , std::enable_shared_from_this<actor_instance<T>> {
    template <class Executor>
    actor_instance(Executor e, std::string name)
        : _q(std::move(e)), _name(std::move(name)) {}

    template <class... Args>
    void initialize(Args&&... args) {
        // We want to construct the object instance in the executor where it will be running. We
        // cannot initialize in the constructor because `shared_from_this` will throw
        // `bad_weak_ptr`.
        //
        // Unfortunately we cannot use schedule() here as it would dereference the std::optional when it
        // doesn't contain a value, and that's UB...
        stlab::async(executor(), [_weak = this->weak_from_this()](auto&&... args){
            if (auto self = _weak.lock()) {
                self->_instance._x = T(std::forward<Args>(args)...);
            }
        }, std::forward<Args>(args)...).detach();
    }

    auto set_name(std::string&& name) { _name = std::move(name); }

    template <typename F>
    auto entask(F&& f) {
        return [_f = std::forward<F>(f), _this = this->shared_from_this()
#ifndef NDEBUG
                , _id = this->id()
#endif // NDEBUG
                ](auto&&... args) mutable {
            // tasks that are "entasked" for an actor must be executed on that same actor, or
            // Bad Things could happen, namely, a data race between this task and any other
            // task(s) the original actor may run simultaneously.
            //
            // If you find yourself here, you have created a task intending it for one actor,
            // but have accidentally tried to execute it on another (including no actor).
            assert(_id == stlab::this_actor::get_id());

            try {
                if constexpr (std::is_same_v<T, void>) {
                    return std::move(_f)(std::forward<decltype(args)>(args)...);
                } else {
                    return std::move(_f)(const_cast<T&>(*(_this->_instance._x)), std::forward<decltype(args)>(args)...);
                }
            } catch (const std::exception& error) {
                const char* what = error.what();
                (void)what;
                assert(!"Uncaught actor exception");
                throw;
            } catch (...) {
                assert(!"Uncaught actor exception");
                throw;
            }
        };
    }

    template <typename F>
    auto operator()(F&& f) {
        return stlab::async(executor(), entask(std::forward<F>(f)));
    }

    auto executor() {
        return [_this = this->shared_from_this()](auto&& task) {
            _this
                ->_q([_t = std::forward<decltype(task)>(task), _this = _this]() mutable {
                    temp_this_actor tta(_this.get());
                    temp_thread_name ttn(_this->_name.c_str());
                    std::move(_t)();
                })
                .detach();
        };
    }

    value_instance<T> _instance;
    stlab::serial_queue_t _q;
    std::string _name;
};

//------------------------------------------------------------------------------------------------------------------------------------------

} // namespace detail

//------------------------------------------------------------------------------------------------------------------------------------------

/// @hyde-owner fbrereto
/// @brief Serialized, asynchronous access to a resource
template <class T>
class actor {
    std::shared_ptr<detail::actor_instance<T>> _impl;

    /// friend declaration to the free function.
    template <class U>
    friend actor<U> this_actor_instance();

public:
    /// Value type for the class. `actor` will own this instance. If the `T` is `void`, nothing
    /// will be instantiated.
    using value_type = T;

    actor() = default;

    /// @param e The executor where actor lambdas will be scheduled
    /// @param name The name of the executor. While the actor is running, the thread it is executing on
    ///             will be temporarily renamed to this value (if the OS supports it.)
    /// @param args Additional arguments to be passed to the `value_type` of this instance during its
    ///             construction.
    template <class Executor, class... Args>
    actor(Executor e, std::string name, Args&&... args)
        : _impl(std::make_shared<detail::actor_instance<value_type>>(std::move(e),
                                                                     std::move(name))) {
        if constexpr (!std::is_same_v<value_type, void>) {
            _impl->initialize(std::forward<Args>(args)...);
        }
    }

    /// @brief Sets the name of the actor to something else.
    /// @param name The incoming name to use from here on out.
    auto set_name(std::string name) { _impl->set_name(std::move(name)); }

    /// @brief Schedule a task for the actor to execute.
    /// @note This routine has identical semantics to `operator()`.
    /// @param f The function to execute. Note that the first parameter to this function must be `T&`,
    ///          and will reference the instance owned by the actor.
    template <typename F>
    auto schedule(F&& f) {
        return (*_impl)(std::forward<F>(f));
    }

    /// @brief Schedule a task for the actor to execute.
    /// @note This routine has identical semantics to `schedule`.
    /// @param f The function to execute. Note that the first parameter to this function must be `T&`,
    ///          and will reference the instance owned by the actor.
    template <typename F>
    auto operator()(F&& f) {
        return (*_impl)(std::forward<F>(f));
    }

    /// @brief Get the unique `actor_id` of this actor.
    auto get_id() const { return _impl->id(); }

    /// This is a nullary task executor, namely, you will not get a reference to the task
    /// local data this actor owns. If you want access that data using this executor, wrap the task
    /// via `actor<T>::entask`. For example:
    ///
    /// ```
    ///     auto f = async(actor.executor(), actor.entask([](auto& data){ /*...*/ }));
    /// ```
    /// @brief Get a nullary task executor for this actor.
    auto executor() { return _impl->executor(); }

    /// @brief Obtain a nullary lambda that can access the `actor`'s task local data.
    /// @return a nullary lambda that, when invoked, will receive the `actor`'s task local data as
    /// its first argument.

    template <typename F>
    auto entask(F&& f) {
        return _impl->entask(std::forward<decltype(f)>(f));
    }

    friend bool operator==(const actor& x, const actor& y) { return x._impl == y._impl; }
    friend bool operator!=(const actor& x, const actor& y) { return !(x == y); }
};

//------------------------------------------------------------------------------------------------------------------------------------------

/// In the event the routine is called outside the context of a running actor, this routine will
/// return the `actor_id` equivalent of `0`.
/// @brief Get the `actor_id` of the currently running actor.
/// @hyde-owner fbrereto
inline actor_id this_actor_id() {
    detail::actor_instance_base* base = detail::this_actor_accessor();
    return base ? base->id() : actor_id{0};
}

/// Get the currently running actor.
/// @hyde-owner fbrereto
/// @throw `std::runtime_error` in the event the routine is called outside the context of a running actor.
template <class U>
actor<U> this_actor_instance() {
    detail::actor_instance_base* base = detail::this_actor_accessor();
    if (!base) {
        throw std::runtime_error("Not in an actor");
    }
    detail::actor_instance<U>& instance = dynamic_cast<detail::actor_instance<U>&>(*base);
    actor<U> result;
    result._impl = instance.shared_from_this();
    return result;
}

/// @brief Determine if the caller is currently in an `actor` execution context.
/// @hyde-owner fbrereto
template <class T>
bool in_actor() {
    // I could make this a little faster by imitating `this_actor` right up to the instance cast.
    return this_actor_instance<T>() != actor<T>();
}

//------------------------------------------------------------------------------------------------------------------------------------------

/// @brief Get the `actor_id` of the currently running actor.
/// @hyde-owner fbrereto
/// @note This routine has identical semantics to `this_actor_id`.
inline actor_id this_actor::get_id() { return this_actor_id(); }

/// @brief Get the currently running actor.
/// @hyde-owner fbrereto
/// @note This routine has identical semantics to `stlab::this_actor<T>()`.
template <typename T>
decltype(auto) this_actor::get() {
    return stlab::this_actor_instance<T>();
}

//------------------------------------------------------------------------------------------------------------------------------------------

} // namespace v1
} // namespace stlab

//------------------------------------------------------------------------------------------------------------------------------------------

#if __clang__
#pragma clang diagnostic pop
#endif // __clang__

//------------------------------------------------------------------------------------------------------------------------------------------

#endif // STLAB_CONCURRENCY_ACTOR_HPP

//------------------------------------------------------------------------------------------------------------------------------------------
