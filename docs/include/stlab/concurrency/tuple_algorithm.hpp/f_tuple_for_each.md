---
layout: function
title: tuple_for_each
hyde:
  owner: sean-parent
  brief: __INLINED__
  tags:
    - function
  inline:
    brief:
      - Applies the given predicate on all tuple elements
  defined_in_file: stlab/concurrency/tuple_algorithm.hpp
  overloads:
    "template <typename T, typename Op>\nvoid tuple_for_each(T &, Op)":
      arguments:
        - description: __OPTIONAL__
          name: t
          type: T &
        - description: __OPTIONAL__
          name: op
          type: Op
      description: __INLINED__
      inline:
        description:
          - Applies the given predicate on all tuple elements
      return: __OPTIONAL__
      signature_with_names: "template <typename T, typename Op>\nvoid tuple_for_each(T & t, Op op)"
  namespace:
    - stlab
    - v3
---
