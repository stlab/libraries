---
layout: function
title: filter_fullorder_range
owner: fosterbrereton
brief: Get a range of filter iterators for a given range
tags:
  - function
defined_in_file: forest.hpp
overloads:
  "template <class R, typename P>\nauto filter_fullorder_range(R &, P)":
    arguments:
      - description: __OPTIONAL__
        name: x
        type: R &
      - description: __OPTIONAL__
        name: p
        type: P
    description: Mutable variant
    return: __OPTIONAL__
    signature_with_names: "template <class R, typename P>\nauto filter_fullorder_range(R & x, P p)"
  "template <class R, typename P>\nauto filter_fullorder_range(const R &, P)":
    arguments:
      - description: __OPTIONAL__
        name: x
        type: const R &
      - description: __OPTIONAL__
        name: p
        type: P
    description: Const variant
    return: __OPTIONAL__
    signature_with_names: "template <class R, typename P>\nauto filter_fullorder_range(const R & x, P p)"
namespace:
  - stlab
---
