/**************************************************************************************************/

#ifndef STLAB_FOREST_ALGORITHMS_HPP
#define STLAB_FOREST_ALGORITHMS_HPP

/**************************************************************************************************/

// stlab
#include <stlab/concurrency/optional.hpp>
#include <stlab/forest.hpp>

/**************************************************************************************************/

namespace stlab {

/**************************************************************************************************/

namespace forests {

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

template <class Container>
struct transcribe_iterator {
    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using difference_type = void;
    using pointer = void;
    using reference = void;
    using container_type = Container;

    transcribe_iterator(Container& c, typename Container::iterator i) : _c{&c}, _i{std::move(i)} {}

    constexpr auto& operator*() { return *this; }
    constexpr auto& operator++() {
        ++_i;
        return *this;
    }
    constexpr auto operator++(int) {
        auto result{*this};
        ++_i;
        return result;
    }

    constexpr auto& operator=(const typename Container::value_type& value) {
        _i = _c->insert(_i, value);
        return *this;
    }

    constexpr auto& operator=(typename Container::value_type&& value) {
        _i = _c->insert(_i, std::move(value));
        return *this;
    }

    constexpr auto trailing() { _i = trailing_of(_i); }

private:
    Container* _c;
    typename Container::iterator _i;
};

template <class Container>
auto transcriber(Container& c) {
    return transcribe_iterator<Container>(c, c.begin());
}

/**************************************************************************************************/

template <class I, class O, class P, class UP>
auto transcribe(I first, I last, O out, P proj, UP pred) {
    for (; first != last; ++first, ++out) {
        if (pred(first)) {
            out = proj(*first);
        } else {
            out.trailing();
        }
    }
    return out;
}

template <class R, class O, class P, class UP>
auto transcribe(const R& range, O out, P proj, UP pred) {
    return transcribe(std::cbegin(range), std::cend(range), std::move(out), std::move(proj),
                      std::move(pred));
}

template <class I, class O, class P>
auto transcribe(I first, I last, O out, P proj) {
    return transcribe(std::move(first), std::move(last), std::move(out), std::move(proj),
                      [](const auto& p) { return is_leading(p); });
}

template <class R, class O, class P>
auto transcribe(const R& range, O out, P proj) {
    return transcribe(std::cbegin(range), std::cend(range), std::move(out), std::move(proj));
}

/**************************************************************************************************/

template <class I, // models ForestFullorderIterator
          class O> // models OutputIterator
auto flatten(I first, I last, O out) {
    for (; first != last; ++first) {
        if (is_leading(first)) {
            *out++ = *first;
        } else {
            *out++ = nullopt;
        }
    }
    return out;
}

/**************************************************************************************************/

template <class I, // I models ForwardIterator; I::value_type == stlab::optional<T>
          class F> // F models Forest
auto unflatten(I first, I last, F& f) {
    return forests::transcribe(
        first, last, transcriber(f), [](const auto& x) { return *x; },
        [](const auto& p) { return *p; });
}

/**************************************************************************************************/

} // namespace forests

/**************************************************************************************************/

} // namespace stlab

/**************************************************************************************************/

#endif // STLAB_FOREST_ALGORITHMS_HPP

/**************************************************************************************************/
