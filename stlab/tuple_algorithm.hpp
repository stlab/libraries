/*-
Copyright 2016 Felix Petriconi
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/**************************************************************************************************/
#ifndef STLAB_TUPLE_ALGORITHM_HPP
#define STLAB_TUPLE_ALGORITHM_HPP

#include <tuple>

namespace stlab
{
    namespace detail
    {
        template<std::size_t I, std::size_t L, typename T, typename Op>
        struct tuple_find_impl
        {
            static auto find(const T& t, Op op) {
                if (op(std::get<I>(t)))
                    return I;
                return tuple_find_impl<I+1, L, T, Op>::find(t, op);
            }
        };


        template<std::size_t L, typename T, typename Op>
        struct tuple_find_impl<L, L, T, Op> {
            static auto find(const T& t, Op op) {
                return L;
            }
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
        struct tuple_for_each_impl<L, L, T, Op> {
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

    }


    template<typename T, typename Op>
    std::size_t tuple_find(const T& t, Op op) {
        if (std::tuple_size<T>::value == 0)
            return 1;
        return detail::tuple_find_impl<0, std::tuple_size<T>::value, T, Op>::find(t, op);
    }

    template<typename T, typename Op>
    void tuple_for_each(T& t, Op op) {
        detail::tuple_for_each_impl<0, std::tuple_size<T>::value, T, Op>::for_each(t, op);
    }

    template <typename T, typename Op>
    std::size_t tuple_min_element(const T& t, Op op)
    {
        if (std::tuple_size<T>::value == 0)
            return 1;
        if (std::tuple_size<T>::value == 1)
            return 0;

        return detail::tuple_min_element_impl<0, std::tuple_size<T>::value - 1, T, Op>::min_element(t, op);
    }
}

#endif