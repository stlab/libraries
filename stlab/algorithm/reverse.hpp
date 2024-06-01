/*
    Copyright 2013 Adobe
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/
/**************************************************************************************************/

#ifndef STLAB_ALGORITHM_REVERSE_HPP
#define STLAB_ALGORITHM_REVERSE_HPP

#include <algorithm>
#include <utility>

#include <stlab/iterator/concepts.hpp>
#include <stlab/iterator/set_next.hpp>

/**************************************************************************************************/

namespace stlab {

/**************************************************************************************************/

namespace unsafe {

/**************************************************************************************************/

template <STLAB_CONCEPT(FORWARD_NODE_ITERATOR) I> // I models NodeIterator
auto reverse_append(I first, I last, I result) -> I {
    while (first != last) {
        I prior(first);
        ++first;
        stlab::unsafe::set_next(prior, result);
        result = prior;
    }
    return result;
}

template <typename R, // R models NodeRange
          typename I> // I models NodeIterator
inline auto reverse_append(R& range, I result) -> I {
    return stlab::unsafe::reverse_append(std::begin(range), std::end(range), result);
}

template <typename I> // I models NodeIterator
inline auto reverse_nodes(I first, I last) -> I {
    return stlab::unsafe::reverse_append(first, last, last);
}

template <typename R> // R models NodeRange
inline auto reverse_nodes(R& range) -> typename R::iterator {
    return stlab::unsafe::reverse_nodes(std::begin(range), std::end(range));
}

/**************************************************************************************************/

} // namespace unsafe

/**************************************************************************************************/

template <class BidirectionalRange>
inline void reverse(BidirectionalRange& range) {
    std::reverse(std::begin(range), std::end(range));
}

template <class BidirectionalRange, class OutputIterator>
inline void reverse_copy(BidirectionalRange& range, OutputIterator result) {
    std::reverse_copy(std::begin(range), std::end(range), result);
}

template <class BidirectionalRange, class OutputIterator>
inline void reverse_copy(const BidirectionalRange& range, OutputIterator result) {
    std::reverse_copy(std::begin(range), std::end(range), result);
}

/**************************************************************************************************/

template <typename I> // I models BidirectionalIterator
auto reverse_until(I f, I m, I l) -> std::pair<I, I> {
    while (f != m && m != l) {
        --l;

        std::iter_swap(f, l);

        ++f;
    }

    return std::pair<I, I>(f, l);
}

/**************************************************************************************************/

} // namespace stlab

/**************************************************************************************************/

#endif // STLAB_ALGORITHM_REVERSE_HPP

/**************************************************************************************************/
