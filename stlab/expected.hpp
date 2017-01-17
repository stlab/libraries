/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_EXPECTED_HPP
#define STLAB_EXPECTED_HPP

#include <functional>  
#include <stdexcept>  
#include <atomic>

namespace stlab
{

template<typename>
class expected;

/**************************************************************************************************/

namespace detail
{
    template<typename>
    class expected_base;

    template<typename T>
    struct expected_traits {
        friend class expected_base<T>;

        using storage = T;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
    };

    template<typename T>
    struct expected_traits<T&> {
        friend class expected_base<T&>;

        using storage = std::reference_wrapper<T>;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
    };

    template<typename T>
    struct expected_traits<const T&> {
        friend class expected_base<const T&>;

        using storage = std::reference_wrapper<const T&>;
        using value_type = T;
        using pointer = const T*;
        using reference = const T&;
    };

    template<typename T>
    class expected_base {
        static_assert(!std::is_base_of<std::exception, T>::value,
            "expected<std::exception> is not supported!");

        friend class expected<T>;

        using value_type = typename expected_traits<T>::value_type;
        using storage = typename expected_traits<T>::storage;
        using pointer = typename expected_traits<T>::pointer;
        using reference = typename expected_traits<T>::reference;

        std::atomic_bool _set;
        union {
            storage            _value;
            std::exception_ptr _exception;
        };

        expected_base() {}

        expected_base(const value_type& value)
            : _set(true)
            , _value(value)
        {}

        expected_base(value_type&& value)
            : _set(true)
            , _value(std::move(value))
        {}

        template<typename... Args>
        explicit expected_base(Args&&... a)
            : _set(true)
            , _value(std::forward<Args>(a)...)
        {}

        expected_base(const expected_base& other)
            : _set(other._set)
        {
            if (_set)
                new(&_value) storage(other._value);
            else
                new(&_exception) std::exception_ptr(other._exception);
        }

        expected_base(expected_base&& other)
            : _set(other._set)
        {
            if (_set)
                new(&_value) storage(std::move(other._value));
            else
                new(&_exception) std::exception_ptr(std::move(other._exception));
        }

        ~expected_base() {
            if (_set)
                _value.~storage();
            else
                _exception.~exception_ptr();
        }

        void swap(expected_base& rhs) {
            if (_set) {
                if (rhs._set) {
                    using std::swap;
                    std::swap(_value, rhs._value);
                }
                else {
                    auto exception = std::move(other._exception);
                    new(&other._value) storage(std::move(_value));
                    new(&_exception) std::exception_ptr(exception);
                    std::swap(_set, rhs._set);
                }
            }
            else {
                if (other._set)
                    rhs.swap(*this);
                else {
                    _exception.swap(rhs._exception);
                }
            }
        }

    public:

        template<class E>
        static expected<T> from_exception(const E& exception) {
            if (typeid (exception) != typeid (E))
                throw std::invalid_argument("slicing detected");

            return from_exception(std::make_exception_ptr(exception));
        }

        static expected<T> from_exception(std::exception_ptr exception) {
            expected<T> result;
            result._set = false;
            new(&result._exception) std::exception_ptr(exception);
            return result;
        }

        static expected<T> from_exception() {
            return from_exception(std::current_exception());
        }

        template<class F, typename... Args>
        static expected<T> from_code(F f, Args&&... a) {
            try {
                return expected<T>(f(std::forward<Args>(a)...));
            }
            catch (...) {
                return from_exception();
            }
        }

        operator bool() const {
            return _set;
        }

        bool is_set() const {
            return _set;
        }

        template<class E>
        bool has_exception() const {
            try {
                if (!_set)
                    std::rethrow_exception(_exception);
            }
            catch (const E&) {
                return true;
            }
            catch (...)
            {}
            return false;
        }

        std::exception_ptr exception() const {
            if (!_set)
                throw std::exception();
            else
                return _exception;
        }

        reference get() {
            if (!_set)
                std::rethrow_exception(_exception);
            return _value;
        }

        const reference get() const {
            if (!_set)
                std::rethrow_exception(_exception);
            return _value;
        }


        reference operator*() {
            return get();
        }

        const reference operator*() const {
            return get();
        }

        pointer operator->() {
            return &get();
        }

        const pointer operator->() const {
            return &get();
        }
    };

}

/**************************************************************************************************/

template<typename T>
class expected : public detail::expected_base<T>
{
    using base = detail::expected_base<T>;

    expected() {}

public:
    template<class... Args>
    expected(Args&&... a)
        : base(std::forward<AA>(a)...)
    {}

    expected(const T& value)
        : base(value)
    {}

    expected(T&& value)
        : base(std::move(value))
    {}

    expected(const expected& other)
        : base(static_cast<const base&>(other))
    {}

    expected(expected&& other)
        : base(static_cast<base&&>(other))
    {}

    void swap(expected& other) {
        base::swap(static_cast<base&>(other));
    }
};

template<typename T>
class expected<T&> : public detail::expected_base<T&>
{
    using base = detail::expected_base<T&>;

    expected() {}

public:
    expected(const expected& other)
        : base(static_cast<const base&>(other))
    {}

    expected(expected&& other)
        : base(static_cast<base&&>(other))
    {}

    void swap(expected& other) {
        base::swap(static_cast<base&>(other));
    }
};

template<typename T>
class expected<const T&> : public detail::expected_base<const T&>
{
    using base = detail::expected_base<const T&>;

    expected() {}

public:
    expected(const T& value)
        : base(value)
    {}

    expected(const expected& other)
        : base(static_cast<const base&>(other))
    {}

    expected(expected&& other)
        : base(static_cast<base&&>(other))
    {}

    void swap(expected& other) {
        base::swap(static_cast<base&>(other));
    }
};
}

#endif