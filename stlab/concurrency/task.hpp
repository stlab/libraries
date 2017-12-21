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
    static constexpr std::size_t small_object_size = 256 - sizeof(void*);

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

    struct vtable_type {
        void (*dtor)(void*);
        void (*move_ctor)(void*, void*) noexcept;
        R (*invoke)(void*, Args&&...);
        const std::type_info& (*target_type)(const void*)noexcept;
        void* (*pointer)(void*)noexcept;
        const void* (*const_pointer)(const void*)noexcept;
    };

    // TODO - we don't create an empty so push this empty vtable into concept
    struct empty {
        empty() noexcept = delete;

        static void dtor(void*) { }
        static void move_ctor(void*, void*) noexcept { }
        static auto invoke(void*, Args&&...) -> R { throw std::bad_function_call(); }
        static auto target_type(const void*) noexcept -> const std::type_info& {
            return typeid(void);
        }
        static auto pointer(void*) noexcept -> void* { return nullptr; }
        static auto const_pointer(const void*) noexcept -> const void* { return nullptr; }

        static constexpr vtable_type _vtable = {
                dtor, move_ctor, invoke, target_type, pointer, const_pointer};
    };

    template <class F, bool Small>
    struct model;

    template <class F>
    struct model<F, true> {
        template <class G> // for forwarding
        model(G&& f) : _f(std::forward<G>(f)) {}
        model(model&&) noexcept = delete;

        static void dtor(void* self) { static_cast<model*>(self)->~model(); }
        static void move_ctor(void* self, void* p) noexcept {
            new (p) model(std::move(static_cast<model*>(self)->_f));
        }
        static auto invoke(void* self, Args&&... args) -> R {
            return std::move(static_cast<model*>(self)->_f)(std::forward<Args>(args)...);
        }
        static auto target_type(const void*) noexcept -> const std::type_info& {
            return typeid(F);
        }
        static auto pointer(void* self) noexcept -> void* {
            return &static_cast<model*>(self)->_f;
        }
        static auto const_pointer(const void* self) noexcept -> const void* {
            return &static_cast<const model*>(self)->_f;
        }

        static vtable_type* vtable() {
            static vtable_type _vtable = {
                dtor, move_ctor, invoke, target_type, pointer, const_pointer};
            return &_vtable;
        }

        F _f;
    };

    template <class F>
    struct model<F, false> {
        template <class G> // for forwarding
        model(G&& f) : _p(std::make_unique<F>(std::forward<G>(f))) {}
        model(model&&) noexcept = default;

        static void dtor(void* self) { static_cast<model*>(self)->~model(); }
        static void move_ctor(void* self, void* p) noexcept {
            new (p) model(std::move(*static_cast<model*>(self)));
        }
        static auto invoke(void* self, Args&&... args) -> R {
            return std::move(*static_cast<model*>(self)->_p)(std::forward<Args>(args)...);
        }
        static auto target_type(const void*) noexcept -> const std::type_info& {
            return typeid(F);
        }
        static auto pointer(void* self) noexcept -> void* {
            return static_cast<model*>(self)->_p.get();
        }
        static auto const_pointer(const void* self) noexcept -> const void* {
            return static_cast<const model*>(self)->_p.get();
        }

        static vtable_type* vtable() {
            static vtable_type _vtable = {
                dtor, move_ctor, invoke, target_type, pointer, const_pointer};
            return &_vtable;
        }

        std::unique_ptr<F> _p;
    };

    struct concept {
        const vtable_type* _vtable_ptr = &empty::_vtable;
        std::aligned_storage_t<small_object_size> _model;

        constexpr concept() noexcept = default;

        template <class F>
        concept(F&& f) {
            using f_t = std::decay_t<F>;
            using model_t = model<f_t, sizeof(model<f_t, true>) <= small_object_size>;

            if (is_empty(f)) return;

            new (&_model) model_t(std::forward<F>(f));
            _vtable_ptr = model_t::vtable();

        }
        ~concept() { _vtable_ptr->dtor(&_model); }
        concept(concept&& x) noexcept : _vtable_ptr(x._vtable_ptr) {
            _vtable_ptr->move_ctor(&x._model, &_model);
        }

        concept& operator=(concept&& x) noexcept {
            _vtable_ptr->dtor(&_model);
            _vtable_ptr = x._vtable_ptr;
            _vtable_ptr->move_ctor(&x._model, &_model);
        }

        R invoke(Args&&... args) { return _vtable_ptr->invoke(&_model, std::forward<Args>(args)...); }
        const std::type_info& target_type() const noexcept {
            return _vtable_ptr->target_type(&_model);
        }
        void* pointer() noexcept { return _vtable_ptr->pointer(&_model); }
        const void* pointer() const noexcept { return _vtable_ptr->const_pointer(&_model); }
    };

    concept _self;

public:
    using result_type = R;

    constexpr task() noexcept = default;
    constexpr task(std::nullptr_t) noexcept : task() {}
    task(const task&) = delete;
    task(task&& x) noexcept = default;

    template <class F>
    task(F&& f) : _self(std::forward<F>(f)) { }

    ~task() = default;

    task& operator=(const task&) = delete;
    task& operator=(task&) = delete;

    task& operator=(task&&) noexcept = default;

    task& operator=(std::nullptr_t) noexcept {
        _self = concept();
        return *this;
    }

    template <class F>
    task& operator=(F&& f) {
        return *this = task(std::forward<F>(f));
    }

    void swap(task& x) noexcept { std::swap(*this, x); }

    explicit operator bool() const { return _self.pointer() != nullptr; }

    const std::type_info& target_type() const { return _self.target_type(); }

    template <class T>
    T* target() {
        return (target_type() == typeid(T)) ? static_cast<T*>(_self.pointer()) : nullptr;
    }

    template <class T>
    const T* target() const {
        return (target_type() == typeid(T)) ? static_cast<const T*>(_self.pointer()) : nullptr;
    }

    R operator()(Args... args) { return _self.invoke(std::forward<Args>(args)...); }

    friend inline void swap(task& x, task& y) { return x.swap(y); }
    friend inline bool operator==(const task& x, std::nullptr_t) { return !static_cast<bool>(x); }
    friend inline bool operator==(std::nullptr_t, const task& x) { return !static_cast<bool>(x); }
    friend inline bool operator!=(const task& x, std::nullptr_t) { return static_cast<bool>(x); }
    friend inline bool operator!=(std::nullptr_t, const task& x) { return static_cast<bool>(x); }
};

template <class R, class... Args>
const typename task<R(Args...)>::vtable_type task<R(Args...)>::empty::_vtable;

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

