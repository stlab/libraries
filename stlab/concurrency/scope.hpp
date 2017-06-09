/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_SCOPE_HPP
#define STLAB_SCOPE_HPP

/**************************************************************************************************/

namespace stlab {

/**************************************************************************************************/

inline namespace v1 {

/**************************************************************************************************/

template <typename Lock>
class scope {
    using mutex_type = typename Lock::mutex_type;
    using lock_type = Lock;

    lock_type _l;

public:
    template <typename... Args>
    explicit scope(Args&&... args) : _l(std::forward<Args>(args)...) { }

    template <typename F>
    void operator()(F&& f) {
        std::forward<F>(f)();
    }
};

/**************************************************************************************************/

} // namespace v1

/**************************************************************************************************/

} // namespace stlab

/**************************************************************************************************/

#endif

/**************************************************************************************************/

