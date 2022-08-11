---
layout: function
title: operator!=
owner: fosterbrereton
brief: Inequality comparator
tags:
  - function
defined_in_file: forest.hpp
overloads:
  bool operator!=(const child_iterator<I> &, const child_iterator<I> &):
    arguments:
      - description: __OPTIONAL__
        name: a
        type: const child_iterator<I> &
      - description: __OPTIONAL__
        name: b
        type: const child_iterator<I> &
    description: child_iterator variant
    return: __OPTIONAL__
    signature_with_names: bool operator!=(const child_iterator<I> & a, const child_iterator<I> & b)
  bool operator!=(const depth_fullorder_iterator<I> &, const depth_fullorder_iterator<I> &):
    arguments:
      - description: __OPTIONAL__
        name: a
        type: const depth_fullorder_iterator<I> &
      - description: __OPTIONAL__
        name: b
        type: const depth_fullorder_iterator<I> &
    description: depth_fullorder_iterator variant
    return: __OPTIONAL__
    signature_with_names: bool operator!=(const depth_fullorder_iterator<I> & a, const depth_fullorder_iterator<I> & b)
  bool operator!=(const edge_iterator<I, Edge> &, const edge_iterator<I, Edge> &):
    arguments:
      - description: __OPTIONAL__
        name: a
        type: const edge_iterator<I, Edge> &
      - description: __OPTIONAL__
        name: b
        type: const edge_iterator<I, Edge> &
    description: edge_iterator variant
    return: __OPTIONAL__
    signature_with_names: bool operator!=(const edge_iterator<I, Edge> & a, const edge_iterator<I, Edge> & b)
  bool operator!=(const filter_fullorder_iterator<I, P> &, const filter_fullorder_iterator<I, P> &):
    arguments:
      - description: __OPTIONAL__
        name: a
        type: const filter_fullorder_iterator<I, P> &
      - description: __OPTIONAL__
        name: b
        type: const filter_fullorder_iterator<I, P> &
    description: filter_fullorder_iterator variant
    return: __OPTIONAL__
    signature_with_names: bool operator!=(const filter_fullorder_iterator<I, P> & a, const filter_fullorder_iterator<I, P> & b)
  bool operator!=(const reverse_fullorder_iterator<I> &, const reverse_fullorder_iterator<I> &):
    arguments:
      - description: __OPTIONAL__
        name: a
        type: const reverse_fullorder_iterator<I> &
      - description: __OPTIONAL__
        name: b
        type: const reverse_fullorder_iterator<I> &
    description: reverse_fullorder_iterator variant
    return: __OPTIONAL__
    signature_with_names: bool operator!=(const reverse_fullorder_iterator<I> & a, const reverse_fullorder_iterator<I> & b)
  "template <class T>\nbool operator!=(const forest<T> &, const forest<T> &)":
    arguments:
      - description: __OPTIONAL__
        name: x
        type: const forest<T> &
      - description: __OPTIONAL__
        name: y
        type: const forest<T> &
    description: forest variant
    return: __OPTIONAL__
    signature_with_names: "template <class T>\nbool operator!=(const forest<T> & x, const forest<T> & y)"
namespace:
  - stlab
---
