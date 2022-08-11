---
layout: class
title: forest<T>
owner: fosterbrereton
brief: A hierarchical, node-based data structure that supports a number of different traversal iterators (forward, reverse, fullorder, preorder, postorder, and more).
tags:
  - class
defined_in_file: forest.hpp
declaration: "template <class T>\nclass stlab::forest;"
typedefs:
  child_iterator:
    definition: stlab::child_iterator<iterator>
    description: child iterator type
  const_child_iterator:
    definition: stlab::child_iterator<const_iterator>
    description: const child iterator type
  const_iterator:
    definition: detail::forest_const_iterator<T>
    description: const fullorder iterator type
  const_pointer:
    definition: const T *
    description: const pointer type
  const_postorder_iterator:
    definition: edge_iterator<stlab::forest::const_iterator, forest_edge::trailing>
    description: const postorder iterator type
  const_preorder_iterator:
    definition: edge_iterator<stlab::forest::const_iterator, forest_edge::leading>
    description: const preorder iterator type
  const_reference:
    definition: const T &
    description: const reference type
  const_reverse_iterator:
    definition: reverse_fullorder_iterator<stlab::forest::const_iterator>
    description: const reverse fullorder iterator type
  difference_type:
    definition: std::ptrdiff_t
    description: difference type
  iterator:
    definition: detail::forest_iterator<T>
    description: fullorder iterator type
  pointer:
    definition: T *
    description: pointer type
  postorder_iterator:
    definition: edge_iterator<stlab::forest::iterator, forest_edge::trailing>
    description: postorder iterator type
  preorder_iterator:
    definition: edge_iterator<stlab::forest::iterator, forest_edge::leading>
    description: preorder iterator type
  reference:
    definition: T &
    description: reference type
  reverse_child_iterator:
    definition: std::reverse_iterator<child_iterator>
    description: reverse child iterator type
  reverse_iterator:
    definition: reverse_fullorder_iterator<stlab::forest::iterator>
    description: reverse fullorder iterator type
  size_type:
    definition: std::size_t
    description: size type
  value_type:
    definition: T
    description: value type
namespace:
  - stlab
---
