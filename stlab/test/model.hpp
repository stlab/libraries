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

} // namespace v1

/**************************************************************************************************/

} // namespace stlab

/**************************************************************************************************/

#endif

/**************************************************************************************************/

