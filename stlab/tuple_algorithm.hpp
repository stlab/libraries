/*
    Copyright 2015 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
    Author: Felix Petriconi
*/

/**************************************************************************************************/

#ifndef STLAB_TUPLE_ALGORITHM_HPP
#define STLAB_TUPLE_ALGORITHM_HPP

#include <tuple>

/**************************************************************************************************/

namespace stlab {

/**************************************************************************************************/

inline namespace v1 {

/**************************************************************************************************/

namespace detail {

/**************************************************************************************************/

template<std::size_t I, std::size_t L, typename T, typename Op>
struct tuple_find_impl
{
    static auto find(const T& t, Op op) {
        if (op(std::get<I>(t))) return I;
        return tuple_find_impl<I+1, L, T, Op>::find(t, op);
    }
};


template<std::size_t L, typename T, typename Op>
struct tuple_find_impl<L, L, T, Op>
{
    static auto find(const T& t, Op op) { return L; }
};


template<std::size_t I, std::size_t L, typename T, typename Op>
struct tuple_for_each_impl
{
    static void for_each(T& t, Op& op) {
        op(std::get<I>(t));
        tuple_for_each_impl<I + 1, L, T, Op>::for_each(t, op);
    }
};


template<std::size_t L, typename T, typename Op>
struct tuple_for_each_impl<L, L, T, Op>
{
    static void for_each(T&, Op&) {}
};

template<std::size_t I, std::size_t L, typename T, typename Op>
struct tuple_min_element_impl
{
    static void min_element(T& t, Op& op) {
        op(std::get<I>(t));
        tuple_min_element_impl<I + 1, L, T, Op>::min_element(t, op);
    }
};

template <std::size_t I, std::size_t L>
struct get_i_impl
{
    template <typename T, typename F, typename D>
    static auto go(T& t, std::size_t index, F&& f, D&& default_v) {
        if (index == I) return std::forward<F>(f)(std::get<I>(t));
        return get_i_impl<I + 1, L>::go(t, index, std::forward<F>(f), std::forward<D>(default_v));
    }
};

template <std::size_t L>
struct get_i_impl<L, L>
{
    template <typename T, typename F, typename D>
    static auto go(T&, std::size_t, F&&, D&& default_v) { return std::forward<D>(default_v); }
};

template <std::size_t I, std::size_t L>
struct void_i_impl
{
    template <typename T, typename F>
    static void go(T& t, std::size_t index, F&& f) {
        if (index == I) std::forward<F>(f)(std::get<I>(t));
        else void_i_impl<I + 1, L>::go(t, index, std::forward<F>(f));
    }
};

template <std::size_t L>
struct void_i_impl<L, L>
{
    template <typename T, typename F>
    static void go(T&, std::size_t, F&&) { }
};
}

/*
 * Finds in a tuple an element that satisfies the given predicate and returns the tuple index.
 * It returns an index beyond the last element if no element satisfies the predicate.
 */
template<typename T, typename Op>
std::size_t tuple_find(const T& t, Op op) {
    if (std::tuple_size<T>::value == 0) return 1;
    return detail::tuple_find_impl<0, std::tuple_size<T>::value, T, Op>::find(t, op);
}

/*
 * Applies the given predicate on all tuple elements
 */
template<typename T, typename Op>
void tuple_for_each(T& t, Op op) {
    detail::tuple_for_each_impl<0, std::tuple_size<T>::value, T, Op>::for_each(t, op);
}

/*
 * Gets from the tuple element at position index the value by applying the given predicate
 * The default value is returned, if the index is equal or greater to tuple_size
 */
template <typename T, typename F, typename D>
auto get_i(T& t, std::size_t index, F&& f, D&& default_v)
{
    return detail::get_i_impl<0, std::tuple_size<T>::value>::go(t, index, std::forward<F>(f), std::forward<D>(default_v));
}


/*
* Applies at the tuple element at position index the the given predicate
*/
template <typename T, typename F>
auto void_i(T& t, std::size_t index, F&& f)
{
    return detail::void_i_impl<0, std::tuple_size<T>::value>::go(t, index, std::forward<F>(f));
}

/**************************************************************************************************/

} // namespace v1

/**************************************************************************************************/

} // namespace stlab

/**************************************************************************************************/

#endif