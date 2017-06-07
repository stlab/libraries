/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_TASK_HPP
#define STLAB_TASK_HPP

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

} // namespace v1

/**************************************************************************************************/

} // namespace stlab

/**************************************************************************************************/

#endif

/**************************************************************************************************/
