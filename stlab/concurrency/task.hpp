/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_CONCURRENCY_TASK_HPP
#define STLAB_CONCURRENCY_TASK_HPP

/**************************************************************************************************/

#include <functional>
#include <memory>
#include <type_traits>
#include <utility>

/**************************************************************************************************/

namespace stlab {

/**************************************************************************************************/

inline namespace v1 {
/**************************************************************************************************/

#if STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_WINDOWS
#pragma warning(push)
#pragma warning(disable : 4521) // disable warning multiple copy c'tor
#pragma warning(disable : 4522) // disable warning multiple assignment op
#endif

/*
    tasks are functions with a mutable call operator to support moving items through for single
    invocations.
*/
template <class>
class task;

template <class R, class... Args>
class task<R(Args...)> {
    /*
        REVISIT (sean.parent) : The size of 256 was an arbitrary choice with no data to back it up.
        Desire is to have something large enough to hold the vast majority of lamda expressions.
        A single small heap allocation/deallocation takes about as much time as copying 10K of POD
        data on my MacBook Pro. tasks are move only object so the exepectation is that moving the
        funciton object is proportional to the sizeof the object. We try to construct tasks emplace
        and so they are rarely moved (need to review the code to make sure that is true). The
        concept will consume one word so this gives us 31 words (on a 64 bit machine) for the model.
        Probably excessive but still allows 16 tasks on a cache line

        I welcome empirical data from an actual system on a better size.
    */
    static constexpr std::size_t small_object_size = 256;

    struct concept {
        struct vtable {
            void (*dtor)(concept*);
            void (*move_ctor)(concept*, void*) noexcept;
            R (*invoke)(concept*, Args&&...);
            const std::type_info& (*target_type)(const concept*) noexcept;
            void* (*pointer)(concept*) noexcept;
            const void* (*const_pointer)(const concept*) noexcept;
        };

        const vtable* const _vtable_ptr;

        concept(const vtable* p) : _vtable_ptr(p) {}
        void dtor() { _vtable_ptr->dtor(this); }
        void move_ctor(void* p) noexcept { _vtable_ptr->move_ctor(this, p); }
        R invoke(Args&&... args) { return _vtable_ptr->invoke(this, std::forward<Args>(args)...); }
        const std::type_info& target_type() const noexcept {
            return _vtable_ptr->target_type(this);
        }
        void* pointer() noexcept { return _vtable_ptr->pointer(this); }
        const void* pointer() const noexcept { return _vtable_ptr->const_pointer(this); }
    };

    struct empty;
    struct empty : concept {
        constexpr empty() noexcept : concept(&_vtable) {}

        constexpr static typename concept::vtable _vtable = {
            [](concept* self) { static_cast<empty*>(self)->~empty(); },
            [](concept*, void* p) noexcept { new (p) empty(); },
            [](concept*, Args&&...) -> R { throw std::bad_function_call(); },
            [](const concept*) noexcept->const std::type_info& { return typeid(void); },
            [](concept*) noexcept->void* { return nullptr; },
            [](const concept*) noexcept->const void* { return nullptr; }
        };
    };

    template <class F, bool Small>
    struct model;

    template <class F>
    struct model<F, true> : concept {
        template <class G> // for forwarding
        model(G&& f) : concept(&_vtable), _f(std::forward<G>(f)) {}
        model(model&&) noexcept = delete;

        constexpr static typename concept::vtable _vtable = {
            [](concept* self) { static_cast<model*>(self)->~model(); },
            [](concept* self, void* p) noexcept {
                new (p) model(std::move(static_cast<model*>(self)->_f));
            },
            [](concept* self, Args&&... args) -> R {
                return std::move(static_cast<model*>(self)->_f)(std::forward<Args>(args)...);
            },
            [](const concept*) noexcept->const std::type_info& { return typeid(F); },
            [](concept* self) noexcept->void* { return &static_cast<model*>(self)->_f; },
            [](const concept* self) noexcept->const void* {
                return &static_cast<const model*>(self)->_f;
            }
        };

        F _f;
    };

    template <class F>
    struct model<F, false> : concept {
        template <class G> // for forwarding
        model(G&& f) : concept(&_vtable), _p(std::make_unique<F>(std::forward<G>(f))) {}
        model(model&&) noexcept = default;

        constexpr static typename concept::vtable _vtable = {
            [](concept* self) { static_cast<model*>(self)->~model(); },
            [](concept* self, void* p) noexcept {
                new (p) model(std::move(*static_cast<model*>(self)));
            },
            [](concept* self, Args&&... args) -> R {
                return std::move(*static_cast<model*>(self)->_p)(std::forward<Args>(args)...);
            },
            [](const concept*) noexcept->const std::type_info& { return typeid(F); },
            [](concept* self) noexcept->void* { return static_cast<model*>(self)->_p.get(); },
            [](const concept* self) noexcept->const void* {
                return static_cast<const model*>(self)->_p.get();
            }
        };

        std::unique_ptr<F> _p;
    };

    concept& self() { return reinterpret_cast<concept&>(_data); }
    const concept& self() const { return reinterpret_cast<const concept&>(_data); }

    std::aligned_storage_t<small_object_size> _data;

    // REVISIT (sean.parent) : Use `if constexpr` here when we move to C++17
    template <class F>
    using possibly_empty_t =
        std::integral_constant<bool,
                           std::is_pointer<std::decay_t<F>>::value ||
                               std::is_member_pointer<std::decay_t<F>>::value ||
                               std::is_same<std::function<R(Args...)>, std::decay_t<F>>::value>;

    template <class F>
    static auto is_empty(const F& f) -> std::enable_if_t<possibly_empty_t<F>::value, bool> {
        return !f;
    }

    template <class F>
    static auto is_empty(const F& f) -> std::enable_if_t<!possibly_empty_t<F>::value, bool> {
        return false;
    }

public:
    using result_type = R;

    constexpr task() noexcept { new (&_data) empty(); }
    constexpr task(std::nullptr_t) noexcept : task() {}
    task(const task&) = delete;
    task(task&& x) noexcept { x.self().move_ctor(&_data); }

    template <class F>
    task(F&& f) {
        using f_t = std::decay_t<F>;
        if (is_empty(f)) {
            new (&_data) empty();
            return;
        }
        try {
            new (&_data)
                model<f_t, sizeof(model<f_t, true>) <= small_object_size>(std::forward<F>(f));
        } catch (...) {
            new (&_data) empty();
            throw;
        }
    }

    ~task() noexcept { self().dtor(); }

    task& operator=(const task&) = delete;
    task& operator=(task&) = delete;

    task& operator=(task&& x) noexcept {
        self().dtor();
        x.self().move_ctor(&_data);
        return *this;
    }

    task& operator=(std::nullptr_t) noexcept {
        self().dtor();
        new (&_data) empty();
        return *this;
    }

    template <class F>
    task& operator=(F&& f) {
        return *this = task(std::forward<F>(f));
    }

    void swap(task& x) noexcept { std::swap(*this, x); }

    explicit operator bool() const { return self().pointer(); }

    const std::type_info& target_type() const { return self().target_type(); }

    template <class T>
    T* target() {
        return (target_type() == typeid(T)) ? static_cast<T*>(self().pointer()) : nullptr;
    }

    template <class T>
    const T* target() const {
        return (target_type() == typeid(T)) ? static_cast<const T*>(self().pointer()) : nullptr;
    }

    R operator()(Args... args) { return self().invoke(std::forward<Args>(args)...); }

    friend inline void swap(task& x, task& y) { return x.swap(y); }
    friend inline bool operator==(const task& x, std::nullptr_t) { return !static_cast<bool>(x); }
    friend inline bool operator==(std::nullptr_t, const task& x) { return !static_cast<bool>(x); }
    friend inline bool operator!=(const task& x, std::nullptr_t) { return static_cast<bool>(x); }
    friend inline bool operator!=(std::nullptr_t, const task& x) { return static_cast<bool>(x); }
};

/**************************************************************************************************/

#if STLAB_TASK_SYSTEM == STLAB_TASK_SYSTEM_WINDOWS
#pragma warning(pop)
#endif

} // namespace v1

/**************************************************************************************************/

} // namespace stlab

/**************************************************************************************************/

#endif

/**************************************************************************************************/
