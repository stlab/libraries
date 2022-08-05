---
layout: function
title: is_trailing
owner: fosterbrereton
brief: Determine if an iterator is on the trailing edge of a node
tags:
  - function
defined_in_file: forest.hpp
overloads:
  constexpr bool is_trailing(stlab::forest_edge):
    arguments:
      - description: __OPTIONAL__
        name: e
        type: stlab::forest_edge
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: constexpr bool is_trailing(stlab::forest_edge e)
  "template <class I>\nauto is_trailing(const I &)":
    arguments:
      - description: __OPTIONAL__
        name: i
        type: const I &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class I>\nauto is_trailing(const I & i)"
namespace:
  - stlab
---
