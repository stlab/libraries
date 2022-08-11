---
layout: function
title: is_leading
owner: fosterbrereton
brief: Determine if an iterator is on the leading edge of a node
tags:
  - function
defined_in_file: forest.hpp
overloads:
  auto is_leading(const stlab::forest_edge &):
    arguments:
      - description: __OPTIONAL__
        name: i
        type: const stlab::forest_edge &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: auto is_leading(const stlab::forest_edge & i)
  constexpr bool is_leading(stlab::forest_edge):
    arguments:
      - description: __OPTIONAL__
        name: e
        type: stlab::forest_edge
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: constexpr bool is_leading(stlab::forest_edge e)
  "template <class I>\nauto is_leading(const I &)":
    arguments:
      - description: __OPTIONAL__
        name: i
        type: const I &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class I>\nauto is_leading(const I & i)"
namespace:
  - stlab
---
