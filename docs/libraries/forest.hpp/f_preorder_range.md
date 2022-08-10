---
layout: function
title: preorder_range
owner: fosterbrereton
brief: Get a range of preorder iterators for a given range
tags:
  - function
defined_in_file: forest.hpp
overloads:
  "template <class R>\nauto preorder_range(R &)":
    arguments:
      - description: __OPTIONAL__
        name: x
        type: R &
    description: Mutable variant
    return: __OPTIONAL__
    signature_with_names: "template <class R>\nauto preorder_range(R & x)"
  "template <class R>\nauto preorder_range(const R &)":
    arguments:
      - description: __OPTIONAL__
        name: x
        type: const R &
    description: Const variant
    return: __OPTIONAL__
    signature_with_names: "template <class R>\nauto preorder_range(const R & x)"
namespace:
  - stlab
---
