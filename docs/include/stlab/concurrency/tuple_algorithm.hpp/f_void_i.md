---
layout: function
title: void_i
hyde:
  owner: sean-parent
  brief: __INLINED__
  tags:
    - function
  inline:
    brief:
      - Applies at the tuple element at position index the the given predicate
  defined_in_file: stlab/concurrency/tuple_algorithm.hpp
  overloads:
    "template <typename T, typename F>\nauto void_i(T &, std::size_t, F &&)":
      arguments:
        - description: __OPTIONAL__
          name: t
          type: T &
        - description: __OPTIONAL__
          name: index
          type: std::size_t
        - description: __OPTIONAL__
          name: f
          type: F &&
      description: __INLINED__
      inline:
        description:
          - Applies at the tuple element at position index the the given predicate
      return: __OPTIONAL__
      signature_with_names: "template <typename T, typename F>\nauto void_i(T & t, std::size_t index, F && f)"
  namespace:
    - stlab
    - v3
---
