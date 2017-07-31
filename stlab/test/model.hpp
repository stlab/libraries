/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_MODEL_HPP
#define STLAB_MODEL_HPP

/**************************************************************************************************/

#include <array>
#include <atomic>
#include <iostream>
#include <memory>

/**************************************************************************************************/

namespace stlab {

/**************************************************************************************************/

inline namespace v1 {

/**************************************************************************************************/

template <typename T>
constexpr std::size_t enum_to_size_t(T t) { return static_cast<std::size_t>(t); }

enum class annotate_switch {
    no_operations_counter,
    with_operations_counter
};

enum class operations
{
    ctor,
    copy_ctor,
    move_ctor,
    assign,
    move_assign,
    swap_,
    dtor,
    last_entry
};

template <annotate_switch S = annotate_switch::no_operations_counter>
struct annotate_t
{
    using operations_counter_t = std::array<std::atomic_int, enum_to_size_t(operations::last_entry)>;

    annotate_t() {
        if (S == annotate_switch::with_operations_counter) {
            if (!operations_counter) {
                operations_counter = std::make_shared<operations_counter_t>();
                for (auto &op : *operations_counter) { op = 0; }
            }
            ++(*operations_counter)[enum_to_size_t(operations::ctor)];
        }
        std::cout << "annotate ctor" << std::endl;
    }

    annotate_t(const annotate_t &x) {
        if (S == annotate_switch::with_operations_counter) {
            operations_counter = x.operations_counter;
            ++(*operations_counter)[enum_to_size_t(operations::copy_ctor)];
        }

        std::cout << "annotate copy-ctor" << std::endl;
    }

    annotate_t(annotate_t&& x) noexcept {
        if (S == annotate_switch::with_operations_counter) {
            operations_counter = x.operations_counter; // no move here!
            ++(*operations_counter)[enum_to_size_t(operations::move_ctor)];
        }
        std::cout << "annotate move-ctor" << std::endl;
    }

    annotate_t &operator=(const annotate_t& x) {
        if (S == annotate_switch::with_operations_counter) {
            operations_counter = x.operations_counter;
            ++(*operations_counter)[enum_to_size_t(operations::assign)];
        }
        std::cout << "annotate assign" << std::endl;
        return *this;
    }

    annotate_t &operator=(annotate_t&& x) noexcept {
        if (S == annotate_switch::with_operations_counter) {
            operations_counter = x.operations_counter;
            ++(*operations_counter)[enum_to_size_t(operations::move_assign)];
        }
        std::cout << "annotate move-assign" << std::endl;
        return *this;
    }

    ~annotate_t() {
        if (S == annotate_switch::with_operations_counter)
            ++(*operations_counter)[enum_to_size_t(operations::dtor)];
        std::cout << "annotate dtor" << std::endl;
    }

    friend inline void swap(annotate_t &x, annotate_t &y) {
        if (S == annotate_switch::with_operations_counter) {
            std::swap(x.operations_counter, y.operations_counter);
            if (x.operations_counter) ++(*x.operations_counter)[enum_to_size_t(operations::swap_)];
            if (y.operations_counter) ++(*y.operations_counter)[enum_to_size_t(operations::swap_)];
        }
        std::cout << "annotate swap" << std::endl;
    }

    friend inline bool operator==(const annotate_t &, const annotate_t &) { return true; }

    friend inline bool operator!=(const annotate_t &, const annotate_t &) { return false; }

    std::shared_ptr<operations_counter_t> operations_counter;
};

using annotate = annotate_t<annotate_switch::no_operations_counter>;
using annotate_counted = annotate_t<annotate_switch::with_operations_counter>;

/**************************************************************************************************/

struct regular {
    int _x;

    explicit regular(int x) : _x(x) { std::cout << _x << " ctor" << std::endl; }
    ~regular() { std::cout << _x << " dtor" << std::endl; }

    regular(const regular& rhs) : _x(rhs._x) {
        std::cout << _x << " copy-ctor" << std::endl;
    }
    regular(regular&& rhs) noexcept : _x(std::move(rhs._x))  {
        std::cout << _x << " move-ctor" << std::endl;
        rhs._x = 0;
    }

    regular& operator=(const regular& rhs) {
        std::cout << _x << " assign" << std::endl;
        _x = rhs._x;
        return *this;
    }
    regular& operator=(regular&& rhs) noexcept {
        std::cout << _x << " move-assign" << std::endl;
        _x = std::move(rhs._x);
        rhs._x = 0;
        return *this;
    }

    friend inline void swap(regular& lhs, regular& rhs) {
        std::cout << lhs._x << "/" << rhs._x << " swap " << std::endl;
        std::swap(lhs._x, rhs._x);
    }

    friend inline bool operator==(const regular& lhs, const regular& rhs) { return lhs._x == rhs._x; }
    friend inline bool operator!=(const regular& lhs, const regular& rhs) { return !(lhs == rhs); }

    friend inline bool operator<(const regular& lhs, const regular& rhs) {
        bool result(lhs._x < rhs._x);
        std::cout << lhs._x << " < " << rhs._x << ": " << std::boolalpha << result << std::endl;
        return result;
    }
};

/**************************************************************************************************/

class move_only {
private:
    int _member;
public:
    move_only(int member) : _member(member) {}
    move_only(const move_only&) = delete;
    move_only& operator=(const move_only&) = delete;
    move_only(move_only&&) = default;
    move_only& operator=(move_only&&) = default;
    virtual ~move_only() = default;
    
    int member() { return _member; }
    int member() const { return _member; }
};

/**************************************************************************************************/

} // namespace v1

/**************************************************************************************************/

} // namespace stlab

/**************************************************************************************************/

#endif

/**************************************************************************************************/

