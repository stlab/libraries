---
layout: function
title: pivot
owner: fosterbrereton
brief: Flip the edge of an iterator
tags:
  - function
defined_in_file: forest.hpp
overloads:
  stlab::forest_edge pivot(stlab::forest_edge):
    arguments:
      - description: __OPTIONAL__
        name: e
        type: stlab::forest_edge
    description: Returns the alternate edge from the one passed
    return: __OPTIONAL__
    signature_with_names: stlab::forest_edge pivot(stlab::forest_edge e)
  "template <typename I>\nvoid pivot(I &)":
    arguments:
      - description: __OPTIONAL__
        name: i
        type: I &
    description: Changes the edge of an iterator to its alternate
    return: __OPTIONAL__
    signature_with_names: "template <typename I>\nvoid pivot(I & i)"
namespace:
  - stlab
---
