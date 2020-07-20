/**************************************************************************************************/

#ifndef STLAB_FOREST_ALGORITHMS_HPP
#define STLAB_FOREST_ALGORITHMS_HPP

/**************************************************************************************************/

// stdc++
#include <optional>

// stlab
#include <stlab/forest.hpp>

/**************************************************************************************************/

namespace stlab {

/**************************************************************************************************/
// "Congruent" would be a nice name here, but in geometry that also implies reflection.
template <class Forest1, class Forest2>
bool equal_shape(const Forest1& x, const Forest2& y) {
    if (x.size_valid() && y.size_valid() && x.size() != y.size()) return false;
    auto pos{y.begin()};
    for (auto first(x.begin()), last(x.end()); first != last; ++first, ++pos) {
        if (first.edge() != pos.edge()) return false;
    }
    return true;
}

/**************************************************************************************************/

template <class Forest>
struct forest_insert_iterator {
    using value_type = typename Forest::value_type;
    using iterator_type = typename Forest::iterator;

    explicit forest_insert_iterator(Forest& f) : _f{f}, _p{_f.root()} {}

    auto& operator*() { return *this; }

    auto& operator++() {
        ++_p;
        return *this;
    }

    auto& operator=(value_type&& x) {
        _p = _f.insert(_p, std::forward<value_type>(x));
        return *this;
    }

    void trailing() { _p = trailing_of(_p); }

private:
    Forest& _f;
    iterator_type _p;
};

template <class Forest>
auto forest_inserter(Forest& f) {
    return forest_insert_iterator(f);
}

/**************************************************************************************************/

template <class I, class O, class P, class UP>
auto transform_forest(I first, I last, O out, P&& proj, UP&& pred) {
    for (; first != last; ++first) {
        ++out;
        if (pred(first)) {
            *out = proj(*first);
        } else {
            out.trailing();
        }
    }
    return out;
}

/**************************************************************************************************/

template <class Forest,
          class P,
          class U = decltype(std::declval<P>()(typename Forest::value_type()))>
auto transcribe_forest(const Forest& f, P&& proj) {
    typename Forest::template rebind<U>::type result;
    transform_forest(f.begin(), f.end(), forest_inserter(result), std::forward<P>(proj),
                     [](auto p) { return is_leading(p); });
    return result;
}

/**************************************************************************************************/

template <class I, // models ForestFullorderIterator
          class O> // models OutputIterator
auto flatten(I first, I last, O out) {
    for (; first != last; ++first) {
        if (is_leading(first)) {
            *out++ = *first;
        } else {
            *out++ = std::nullopt;
        }
    }
    return out;
}

/**************************************************************************************************/

template <class I, // I models ForwardIterator; I::value_type == std::optional<T>
          class O> // O models ForestOutputIterator
auto unflatten(I first, I last, O out) {
    return transform_forest(
        first, last, out, [](auto x) { return *x; }, [](auto p) { return *p; });
}

/**************************************************************************************************/

} // namespace stlab

/**************************************************************************************************/

#endif // STLAB_FOREST_ALGORITHMS_HPP

/**************************************************************************************************/
