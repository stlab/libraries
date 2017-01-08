/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef SLABFUTURE_UTILITY_HPP
#define SLABFUTURE_UTILITY_HPP

#if 0

#include <thread>

// usefull makro for debugging
#define STLAB_TRACE(S) \
    printf("%s:%d %d %s\n", __FILE__, __LINE__, (int)std::hash<std::thread::id>()(std::this_thread::get_id()), S);

#endif


namespace stlab
{

/**************************************************************************************************/

template <typename T>
future<T> make_ready_future(T&& x) {
    auto p = package<T(T)>(default_executor(), [](auto&& x) { return x; });
    p.first(x);
    return p.second;
}

inline future<void> make_ready_future() {
    auto p = package<void()>(default_executor(), [](){});
    p.first();
    return p.second;
}

/**************************************************************************************************/

}

#endif //SLABFUTURE_UTILITY_HPP
