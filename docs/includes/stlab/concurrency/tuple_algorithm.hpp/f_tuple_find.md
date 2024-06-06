---
layout: function
title: tuple_find
hyde:
  owner: sean-parent
  brief: __INLINED__
  tags:
    - function
  inline:
    brief:
      - "***********************************************************************************************"
      - Finds in a tuple an element that satisfies the given predicate and returns the tuple index. It returns an index beyond the last element if no element satisfies the predicate.
  defined_in_file: stlab/concurrency/tuple_algorithm.hpp
  overloads:
    "template <typename T, typename Op>\nauto tuple_find(const T &, Op) -> std::size_t":
      arguments:
        - description: __OPTIONAL__
          name: t
          type: const T &
        - description: __OPTIONAL__
          name: op
          type: Op
      description: __INLINED__
      inline:
        description:
          - "***********************************************************************************************"
          - Finds in a tuple an element that satisfies the given predicate and returns the tuple index. It returns an index beyond the last element if no element satisfies the predicate.
      return: __OPTIONAL__
      signature_with_names: "template <typename T, typename Op>\nauto tuple_find(const T & t, Op op) -> std::size_t"
  namespace:
    - stlab
    - v3
---
