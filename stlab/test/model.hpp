/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_MODEL_HPP
#define STLAB_MODEL_HPP

/**************************************************************************************************/

#include <iostream>

/**************************************************************************************************/

namespace stlab {

/**************************************************************************************************/

inline namespace v1 {

/**************************************************************************************************/

struct annotate {
    annotate() { std::cout << "annotate ctor" << std::endl; }
    ~annotate() { std::cout << "annotate dtor" << std::endl; }

    annotate(const annotate&) { std::cout << "annotate copy-ctor" << std::endl; }
    annotate(annotate&&) noexcept { std::cout << "annotate move-ctor" << std::endl; }

    annotate& operator=(const annotate&) {
        std::cout << "annotate assign" << std::endl;
        return *this;
    }
    annotate& operator=(annotate&&) noexcept {
        std::cout << "annotate move-assign" << std::endl;
        return *this;
    }

    friend inline void swap(annotate&, annotate&) { std::cout << "annotate swap" << std::endl; }
    friend inline bool operator==(const annotate&, const annotate&) { return true; }
    friend inline bool operator!=(const annotate&, const annotate&) { return false; }
};

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

} // namespace v1

/**************************************************************************************************/

} // namespace stlab

/**************************************************************************************************/

#endif

/**************************************************************************************************/

