#ifndef STLAB_ITERATOR_CONCEPTS_HPP
#define STLAB_ITERATOR_CONCEPTS_HPP

#define STLAB_CONCEPT(x) STLAB_PRIVATE_CONCEPT_##x

#if __cpp_has_concepts

#include <iterator>

#include <stlab/config.hpp>
#include <stlab/iterator/set_next.hpp>

namespace stlab {
inline namespace STLAB_VERSION_NAMESPACE() {

template <class I>
concept forward_node_iterator = std::forward_iterator<I> && requires(I i) {
    { stlab::unsafe::set_next(i, i) };
};
}
} // namespace stlab

#define STLAB_PRIVATE_CONCEPT_FORWARD_NODE_ITERATOR stlab::forward_node_iterator

#else

#define STLAB_PRIVATE_CONCEPT_FORWARD_NODE_ITERATOR typename

#endif

#endif
