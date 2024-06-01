---
layout: function
title: operator!=
hyde:
  owner: fosterbrereton
  brief: Inequality comparator
  tags:
    - function
  defined_in_file: stlab/forest.hpp
  overloads:
    auto operator!=(const child_iterator<I> &, const child_iterator<I> &) -> bool:
      arguments:
        - description: __OPTIONAL__
          name: a
          type: const child_iterator<I> &
        - description: __OPTIONAL__
          name: b
          type: const child_iterator<I> &
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: auto operator!=(const child_iterator<I> & a, const child_iterator<I> & b) -> bool
    auto operator!=(const depth_fullorder_iterator<I> &, const depth_fullorder_iterator<I> &) -> bool:
      arguments:
        - description: __OPTIONAL__
          name: a
          type: const depth_fullorder_iterator<I> &
        - description: __OPTIONAL__
          name: b
          type: const depth_fullorder_iterator<I> &
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: auto operator!=(const depth_fullorder_iterator<I> & a, const depth_fullorder_iterator<I> & b) -> bool
    auto operator!=(const edge_iterator<I, Edge> &, const edge_iterator<I, Edge> &) -> bool:
      arguments:
        - description: __OPTIONAL__
          name: a
          type: const edge_iterator<I, Edge> &
        - description: __OPTIONAL__
          name: b
          type: const edge_iterator<I, Edge> &
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: auto operator!=(const edge_iterator<I, Edge> & a, const edge_iterator<I, Edge> & b) -> bool
    auto operator!=(const filter_fullorder_iterator<I, P> &, const filter_fullorder_iterator<I, P> &) -> bool:
      arguments:
        - description: __OPTIONAL__
          name: a
          type: const filter_fullorder_iterator<I, P> &
        - description: __OPTIONAL__
          name: b
          type: const filter_fullorder_iterator<I, P> &
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: auto operator!=(const filter_fullorder_iterator<I, P> & a, const filter_fullorder_iterator<I, P> & b) -> bool
    auto operator!=(const reverse_fullorder_iterator<I> &, const reverse_fullorder_iterator<I> &) -> bool:
      arguments:
        - description: __OPTIONAL__
          name: a
          type: const reverse_fullorder_iterator<I> &
        - description: __OPTIONAL__
          name: b
          type: const reverse_fullorder_iterator<I> &
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: auto operator!=(const reverse_fullorder_iterator<I> & a, const reverse_fullorder_iterator<I> & b) -> bool
    "template <class T>\nauto operator!=(const forest<T> &, const forest<T> &) -> bool":
      arguments:
        - description: __OPTIONAL__
          name: x
          type: const forest<T> &
        - description: __OPTIONAL__
          name: y
          type: const forest<T> &
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: "template <class T>\nauto operator!=(const forest<T> & x, const forest<T> & y) -> bool"
  namespace:
    - stlab
---
