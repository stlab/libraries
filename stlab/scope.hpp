/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/

#ifndef STLAB_SCOPE_HPP
#define STLAB_SCOPE_HPP

/**************************************************************************************************/

#include <tuple>
#include <utility>

/**************************************************************************************************/

namespace stlab {

/**************************************************************************************************/

inline namespace v1 {

/**************************************************************************************************/

namespace detail {

template <typename T, typename Tuple, size_t... S>
void scope_call(Tuple&& t, std::index_sequence<S...>) {
    T scoped(std::forward<std::tuple_element_t<S, Tuple>>(std::get<S>(t))...);

    // call the function
    constexpr size_t last_index = std::tuple_size<Tuple>::value - 1;
    std::forward<std::tuple_element_t<last_index, Tuple>>(std::get<last_index>(t))();
}

} // namespace detail

/**************************************************************************************************/

template <typename T, typename... Args>
inline void scope(Args&&... args) {
    detail::scope_call<T>(std::forward_as_tuple(std::forward<Args>(args)...),
                          std::make_index_sequence<sizeof...(args) - 1>());
}

/**************************************************************************************************/

} // namespace v1

/**************************************************************************************************/

} // namespace stlab

/**************************************************************************************************/

#endif

/**************************************************************************************************/

