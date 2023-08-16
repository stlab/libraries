/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_CONCURRENCY_TASK_HPP
#define STLAB_CONCURRENCY_TASK_HPP

/**************************************************************************************************/

#include <algorithm>
#include <cstddef>
#include <functional>
#include <memory>
#include <type_traits>
#include <utility>

#include <stlab/config.hpp>

/**************************************************************************************************/

namespace stlab {

/**************************************************************************************************/

inline namespace v1 {

/**************************************************************************************************/

namespace detail {

/**************************************************************************************************/

template <bool NoExcept, class R, class... Args>
class task_base {
    template <class F>
    constexpr static bool maybe_empty =
        std::is_pointer<std::decay_t<F>>::value || std::is_member_pointer<std::decay_t<F>>::value ||
        std::is_same<std::function<R(Args...)>, std::decay_t<F>>::value;

    template <class F>
    constexpr static auto is_empty(const F& f) -> std::enable_if_t<maybe_empty<F>, bool> {
        return !f;
    }

    template <class F>
    constexpr static auto is_empty(const F&) -> std::enable_if_t<!maybe_empty<F>, bool> {
        return false;
    }

    struct concept_t {
        void (*dtor)(void*) noexcept;
        void (*move_ctor)(void*, void*) noexcept;
        const std::type_info& (*target_type)() noexcept;
        void* (*pointer)(void*) noexcept;
        const void* (*const_pointer)(const void*) noexcept;
    };

    using invoke_t = R (*)(void*, Args...) noexcept(NoExcept);

    template <class F, bool Small>
    struct model;

    template <class F>
    struct model<F, true> {
        template <class G> // for forwarding
        model(G&& f) : _f(std::forward<G>(f)) {}
        model(model&&) noexcept = delete;

        static void dtor(void* self) noexcept { static_cast<model*>(self)->~model(); }
        static void move_ctor(void* self, void* p) noexcept {
            new (p) model(std::move(static_cast<model*>(self)->_f));
        }

        /*
            NOTE (sean-parent): `Args` are _not_ universal references. This is a `concrete`
            interface for the model. Do not add `&&`, that would make it an rvalue reference.
            The `forward<Args>` here is correct. We are forwarding from the client defined
            signature to the actual captured model.
        */

        static auto invoke(void* self, Args... args) noexcept(NoExcept) -> R {
            return (static_cast<model*>(self)->_f)(std::forward<Args>(args)...);
        }

        static auto target_type() noexcept -> const std::type_info& { return typeid(F); }
        static auto pointer(void* self) noexcept -> void* { return &static_cast<model*>(self)->_f; }
        static auto const_pointer(const void* self) noexcept -> const void* {
            return &static_cast<const model*>(self)->_f;
        }
#if defined(__GNUC__) && __GNUC__ < 7 && !defined(__clang__)
        static const concept_t _vtable;
        static const invoke_t _invoke;
#else
        static constexpr concept_t _vtable = {dtor, move_ctor, target_type, pointer, const_pointer};
        static constexpr invoke_t _invoke = invoke;
#endif
        F _f;
    };

    template <class F>
    struct model<F, false> {
        template <class G> // for forwarding
        model(G&& f) : _p(std::make_unique<F>(std::forward<G>(f))) {}
        model(model&&) noexcept = default;

        static void dtor(void* self) noexcept { static_cast<model*>(self)->~model(); }
        static void move_ctor(void* self, void* p) noexcept {
            new (p) model(std::move(*static_cast<model*>(self)));
        }

        /*
            NOTE (sean-parent): `Args` are _not_ universal references. This is a `concrete`
            interface for the model. Do not add `&&`, that would make it an rvalue reference.
            The `forward<Args>` here is correct. We are forwarding from the client defined
            signature to the actual captured model.
        */

        static auto invoke(void* self, Args... args) noexcept(NoExcept) -> R {
            return (*static_cast<model*>(self)->_p)(std::forward<Args>(args)...);
        }

        static auto target_type() noexcept -> const std::type_info& { return typeid(F); }
        static auto pointer(void* self) noexcept -> void* {
            return static_cast<model*>(self)->_p.get();
        }
        static auto const_pointer(const void* self) noexcept -> const void* {
            return static_cast<const model*>(self)->_p.get();
        }

#if defined(__GNUC__) && __GNUC__ < 7 && !defined(__clang__)
        static const concept_t _vtable;
        static const invoke_t _invoke;
#else
        static constexpr concept_t _vtable = {dtor, move_ctor, target_type, pointer, const_pointer};
        static constexpr invoke_t _invoke = invoke;
#endif

        std::unique_ptr<F> _p;
    };

    // empty (default) vtable
    static void dtor(void*) noexcept {}
    static void move_ctor(void*, void*) noexcept {}
    static auto invoke(void*, Args...) noexcept(NoExcept) -> R {
        if constexpr (NoExcept) {
            std::terminate();
        } else {
            throw std::bad_function_call();
        }
    }
    static auto target_type_() noexcept -> const std::type_info& { return typeid(void); }
    static auto pointer(void*) noexcept -> void* { return nullptr; }
    static auto const_pointer(const void*) noexcept -> const void* { return nullptr; }

#if defined(__GNUC__) && __GNUC__ < 7 && !defined(__clang__)
    static const concept_t _vtable;
#else
    static constexpr concept_t _vtable = {dtor, move_ctor, target_type_, pointer, const_pointer};
#endif

    /*
    The layout of this object is going to be:
        _vtable_ptr
        _invoke
        _model

    Because the model is going to be max-aligned, it will leave a gap between the vtable_ptr and
    the _model, we fill that gap by lifting the _invoke pointer into it from the vtable. This means
    invoke calls require one less indirection.

    The size of the model is big enough for 6 pointers or (2 max aligned object - 2 pointers)
    (which would typically be 2 pointers). The rational here is that we want the total object size
    to be a power of 2, for the object but the model store to be large enough to hold 2
    weak-pointers (which is 4 pointers total), so we give things a little extra room.
    */

    static constexpr size_t max_align = alignof(std::max_align_t);
    static constexpr size_t small_size =
        std::max(max_align * 2, sizeof(void*) * 8) - std::max(max_align, sizeof(void*) * 2);

    const concept_t* _vtable_ptr = &_vtable;
    invoke_t _invoke = invoke;
    std::aligned_storage_t<small_size> _model;

public:
    using result_type = R;

    constexpr task_base() noexcept = default;
    constexpr task_base(std::nullptr_t) noexcept : task_base() {}
    task_base(const task_base&) = delete;
    task_base(task_base&& x) noexcept : _vtable_ptr(x._vtable_ptr), _invoke(x._invoke) {
        _vtable_ptr->move_ctor(&x._model, &_model);
    }

    template <class F,
              std::enable_if_t<!NoExcept || noexcept(std::declval<F>()(std::declval<Args>()...)),
                               bool> = true>
    task_base(F&& f) {
        using small_t = model<std::decay_t<F>, true>;
        using large_t = model<std::decay_t<F>, false>;
        using model_t = std::conditional_t<(sizeof(small_t) <= small_size) &&
                                               (alignof(small_t) <= alignof(decltype(_model))),
                                           small_t, large_t>;

        if (is_empty(f)) return;

        new (&_model) model_t(std::forward<F>(f));
        _vtable_ptr = &model_t::_vtable;
        _invoke = &model_t::invoke;
    }

    ~task_base() { _vtable_ptr->dtor(&_model); };

    task_base& operator=(const task_base&) = delete;

    task_base& operator=(task_base&& x) noexcept {
        _vtable_ptr->dtor(&_model);
        _vtable_ptr = x._vtable_ptr;
        _invoke = x._invoke;
        _vtable_ptr->move_ctor(&x._model, &_model);
        return *this;
    }

    task_base& operator=(std::nullptr_t) noexcept { return *this = task_base(); }

    template <class F>
    auto operator=(F&& f)
        -> std::enable_if_t<!NoExcept || noexcept(f(std::declval<Args>()...)), task_base&> {
        return *this = task_base(std::forward<F>(f));
    }

    void swap(task_base& x) noexcept { std::swap(*this, x); }

    explicit operator bool() const { return _vtable_ptr->const_pointer(&_model) != nullptr; }

    const std::type_info& target_type() const noexcept { return _vtable_ptr->target_type(); }

    template <class T>
    T* target() {
        return (target_type() == typeid(T)) ? static_cast<T*>(_vtable_ptr->pointer(&_model)) :
                                              nullptr;
    }

    template <class T>
    const T* target() const {
        return (target_type() == typeid(T)) ?
                   static_cast<const T*>(_vtable_ptr->const_pointer(&_model)) :
                   nullptr;
    }

    template <class... Brgs>
    auto operator()(Brgs&&... brgs) noexcept(NoExcept) {
        return _invoke(&_model, std::forward<Brgs>(brgs)...);
    }

    friend inline bool operator==(const task_base& x, std::nullptr_t) {
        return !static_cast<bool>(x);
    }
    friend inline bool operator==(std::nullptr_t, const task_base& x) {
        return !static_cast<bool>(x);
    }
    friend inline bool operator!=(const task_base& x, std::nullptr_t) {
        return static_cast<bool>(x);
    }
    friend inline bool operator!=(std::nullptr_t, const task_base& x) {
        return static_cast<bool>(x);
    }
};

#if STLAB_CPP_VERSION_LESS_THAN(17)

// In C++17 constexpr implies inline and these definitions are deprecated

#if defined(__GNUC__) && __GNUC__ < 7 && !defined(__clang__)
template <class R, class... Args, bool NoExcept>
const typename task_base<R(Args...) noexcept(NoExcept)>::concept_t
    task_base<R(Args...) noexcept(NoExcept)>::_vtable = {dtor, move_ctor, target_type_, pointer,
                                                         const_pointer};

template <class R, class... Args, bool NoExcept>
const typename task_base<R(Args...) noexcept(NoExcept)>::invoke_t
    task_base<R(Args...) noexcept(NoExcept)>::_invoke = _invoke;
#else
template <class R, class... Args, bool NoExcept>
const typename task_base<R(Args...) noexcept(NoExcept)>::concept_t
    task_base<R(Args...) noexcept(NoExcept)>::_vtable;
#endif

#ifdef _MSC_VER

template <class R, class... Args, bool NoExcept>
template <class F>
const typename task_base<R(Args...) noexcept(NoExcept)>::concept_t
    task_base<R(Args...) noexcept(NoExcept)>::model<F, false>::_vtable;

template <class R, class... Args, bool NoExcept>
template <class F>
const typename task_base<R(Args...) noexcept(NoExcept)>::concept_t
    task_base<R(Args...) noexcept(NoExcept)>::model<F, true>::_vtable;

#else

#if defined(__GNUC__) && __GNUC__ < 7 && !defined(__clang__)

template <class R, class... Args>
template <class F>
const typename task_base<R(Args...)>::concept_t
    task_base<R(Args...)>::template model<F, false>::_vtable = {dtor, move_ctor, target_type,
                                                                pointer, const_pointer};

template <class R, class... Args>
template <class F>
const typename task_base<R(Args...)>::concept_t
    task_base<R(Args...)>::template model<F, true>::_vtable = {dtor, move_ctor, target_type,
                                                               pointer, const_pointer};

#else

template <class R, class... Args>
template <class F>
const typename task_base<R(Args...)>::concept_t task_base<R(Args...)>::model<F, false>::_vtable;

template <class R, class... Args>
template <class F>
const typename task_base<R(Args...)>::concept_t task_base<R(Args...)>::model<F, true>::_vtable;

#endif

#endif

#endif

/**************************************************************************************************/

} // namespace detail

/**************************************************************************************************/

template <class F>
class task;

template <class R, class... Args>
class task<R(Args...)> : detail::task_base<false, R, Args...> {
    using base_type = detail::task_base<false, R, Args...>;

public:
    using base_type::base_type;
    using base_type::target_type;
    using typename base_type::result_type;
    using base_type::operator();
    using base_type::operator bool;
    using base_type::swap;
    using base_type::target;

    friend inline void swap(task& x, task& y) { return x.swap(y); }
    friend inline bool operator==(const task& x, std::nullptr_t) {
        return static_cast<const base_type&>(x) == nullptr;
    }
    friend inline bool operator==(std::nullptr_t, const task& x) { return x == nullptr; }
    friend inline bool operator!=(const task& x, std::nullptr_t) { return !(x == nullptr); }
    friend inline bool operator!=(std::nullptr_t, const task& x) { return !(x == nullptr); }
};

template <class R, class... Args>
class task<R(Args...) noexcept> : detail::task_base<true, R, Args...> {
    using base_type = detail::task_base<true, R, Args...>;

public:
    using base_type::base_type;
    using base_type::target_type;
    using typename base_type::result_type;
    using base_type::operator();
    using base_type::operator bool;
    using base_type::swap;
    using base_type::target;

    friend inline void swap(task& x, task& y) { return x.swap(y); }
    friend inline bool operator==(const task& x, std::nullptr_t) {
        return static_cast<const base_type&>(x) == nullptr;
    }
    friend inline bool operator==(std::nullptr_t, const task& x) { return x == nullptr; }
    friend inline bool operator!=(const task& x, std::nullptr_t) { return !(x == nullptr); }
    friend inline bool operator!=(std::nullptr_t, const task& x) { return !(x == nullptr); }
};

/**************************************************************************************************/

} // namespace v1

/**************************************************************************************************/

} // namespace stlab

/**************************************************************************************************/

#endif

/**************************************************************************************************/
