---
layout: function
title: get_i
hyde:
  owner: sean-parent
  brief: __INLINED__
  tags:
    - function
  inline:
    brief:
      - Gets from the tuple element at position index the value by applying the given predicate The default value is returned, if the index is equal or greater to tuple_size
  defined_in_file: stlab/concurrency/tuple_algorithm.hpp
  overloads:
    "template <typename T, typename F, typename D>\nauto get_i(T &, std::size_t, F, D &&)":
      arguments:
        - description: __OPTIONAL__
          name: t
          type: T &
        - description: __OPTIONAL__
          name: index
          type: std::size_t
        - description: __OPTIONAL__
          name: f
          type: F
        - description: __OPTIONAL__
          name: default_v
          type: D &&
      description: __INLINED__
      inline:
        description:
          - Gets from the tuple element at position index the value by applying the given predicate The default value is returned, if the index is equal or greater to tuple_size
      return: __OPTIONAL__
      signature_with_names: "template <typename T, typename F, typename D>\nauto get_i(T & t, std::size_t index, F f, D && default_v)"
  namespace:
    - stlab
    - v3
---
