/*
    Copyright 2013 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
/**************************************************************************************************/

#ifndef STLAB_COPY_ON_WRITE_HPP
#define STLAB_COPY_ON_WRITE_HPP

/**************************************************************************************************/

#include <atomic>
#include <cassert>
#include <cstddef>
#include <type_traits>
#include <utility>

/**************************************************************************************************/

namespace stlab {

/**************************************************************************************************/

template <typename T> // T models Regular
class copy_on_write {
    struct model {
        std::atomic<std::size_t> _count{1};

        model() noexcept(std::is_nothrow_constructible<T>::value) = default;

        template <class... Args>
        explicit model(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args&&...>) :
            _value(std::forward<Args>(args)...) {}

        T _value;
    };

    model* _self;

    template <class U>
    using disable_copy = std::enable_if_t<!std::is_same_v<std::decay_t<U>, copy_on_write>>*;

    template <typename U>
    using disable_copy_assign =
        std::enable_if_t<!std::is_same_v<std::decay_t<U>, copy_on_write>, copy_on_write&>;

    auto default_model() noexcept(std::is_nothrow_constructible_v<T>) -> model* {
        static model default_s;
        return &default_s;
    }

public:
    /* [[deprecated]] */ using value_type = T;

    using element_type = T;

    copy_on_write() noexcept(std::is_nothrow_constructible_v<T>) {
        _self = default_model();

        // coverity[useless_call]
        ++_self->_count;
    }

    template <class U>
    copy_on_write(U&& x, disable_copy<U> = nullptr) : _self(new model(std::forward<U>(x))) {}

    template <class U, class V, class... Args>
    copy_on_write(U&& x, V&& y, Args&&... args) :
        _self(new model(std::forward<U>(x), std::forward<V>(y), std::forward<Args>(args)...)) {}

    copy_on_write(const copy_on_write& x) noexcept : _self(x._self) {
        assert(_self && "FATAL (sparent) : using a moved copy_on_write object");

        // coverity[useless_call]
        ++_self->_count;
    }
    copy_on_write(copy_on_write&& x) noexcept : _self{std::exchange(x._self, nullptr)} {
        assert(_self && "WARNING (sparent) : using a moved copy_on_write object");
    }

    ~copy_on_write() {
        assert(!_self || (_self->_count > 0) && "FATAL (sparent) : double delete");
        if (_self && (--_self->_count == 0)) {
            if constexpr (std::is_default_constructible_v<element_type>) {
                assert(_self != default_model());
            }
            delete _self;
        }
    }

    auto operator=(const copy_on_write& x) noexcept -> copy_on_write& {
        // self-assignment is not allowed to disable cert-oop54-cpp warning (and is likely a bug)
        assert(this != &x && "self-assignment is not allowed");
        return *this = copy_on_write(x);
    }

    auto operator=(copy_on_write&& x) noexcept -> copy_on_write& {
        auto tmp{std::move(x)};
        swap(*this, tmp);
        return *this;
    }

    template <class U>
    auto operator=(U&& x) -> disable_copy_assign<U> {
        if (_self && unique()) {
            _self->_value = std::forward<U>(x);
            return *this;
        }

        return *this = copy_on_write(std::forward<U>(x));
    }

    auto write() -> element_type& {
        if (!unique()) *this = copy_on_write(read());

        return _self->_value;
    }

    [[nodiscard]] auto read() const noexcept -> const element_type& {
        assert(_self && "FATAL (sparent) : using a moved copy_on_write object");

        return _self->_value;
    }

    operator const element_type&() const noexcept { return read(); }

    auto operator*() const noexcept -> const element_type& { return read(); }

    auto operator->() const noexcept -> const element_type* { return &read(); }

    [[nodiscard]] auto unique() const noexcept -> bool {
        assert(_self && "FATAL (sparent) : using a moved copy_on_write object");

        return _self->_count == 1;
    }
    [[deprecated]] [[nodiscard]] auto unique_instance() const noexcept -> bool { return unique(); }

    [[nodiscard]] auto identity(const copy_on_write& x) const noexcept -> bool {
        assert((_self && x._self) && "FATAL (sparent) : using a moved copy_on_write object");

        return _self == x._self;
    }

    friend inline void swap(copy_on_write& x, copy_on_write& y) noexcept {
        std::swap(x._self, y._self);
    }

    friend inline auto operator<(const copy_on_write& x, const copy_on_write& y) noexcept -> bool {
        return !x.identity(y) && (*x < *y);
    }

    friend inline auto operator<(const copy_on_write& x, const element_type& y) noexcept -> bool {
        return *x < y;
    }

    friend inline auto operator<(const element_type& x, const copy_on_write& y) noexcept -> bool {
        return x < *y;
    }

    friend inline auto operator>(const copy_on_write& x, const copy_on_write& y) noexcept -> bool {
        return y < x;
    }

    friend inline auto operator>(const copy_on_write& x, const element_type& y) noexcept -> bool {
        return y < x;
    }

    friend inline auto operator>(const element_type& x, const copy_on_write& y) noexcept -> bool {
        return y < x;
    }

    friend inline auto operator<=(const copy_on_write& x, const copy_on_write& y) noexcept -> bool {
        return !(y < x);
    }

    friend inline auto operator<=(const copy_on_write& x, const element_type& y) noexcept -> bool {
        return !(y < x);
    }

    friend inline auto operator<=(const element_type& x, const copy_on_write& y) noexcept -> bool {
        return !(y < x);
    }

    friend inline auto operator>=(const copy_on_write& x, const copy_on_write& y) noexcept -> bool {
        return !(x < y);
    }

    friend inline auto operator>=(const copy_on_write& x, const element_type& y) noexcept -> bool {
        return !(x < y);
    }

    friend inline auto operator>=(const element_type& x, const copy_on_write& y) noexcept -> bool {
        return !(x < y);
    }

    friend inline auto operator==(const copy_on_write& x, const copy_on_write& y) noexcept -> bool {
        return x.identity(y) || (*x == *y);
    }

    friend inline auto operator==(const copy_on_write& x, const element_type& y) noexcept -> bool {
        return *x == y;
    }

    friend inline auto operator==(const element_type& x, const copy_on_write& y) noexcept -> bool {
        return x == *y;
    }

    friend inline auto operator!=(const copy_on_write& x, const copy_on_write& y) noexcept -> bool {
        return !(x == y);
    }

    friend inline auto operator!=(const copy_on_write& x, const element_type& y) noexcept -> bool {
        return !(x == y);
    }

    friend inline auto operator!=(const element_type& x, const copy_on_write& y) noexcept -> bool {
        return !(x == y);
    }
};
/**************************************************************************************************/

} // namespace stlab

/**************************************************************************************************/

#endif

/**************************************************************************************************/
