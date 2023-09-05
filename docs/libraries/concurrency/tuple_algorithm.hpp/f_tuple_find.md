---
layout: function
title: tuple_find
hyde:
  owner: __MISSING__
  brief: __INLINED__
  tags:
    - function
  inline:
    brief:
      - "***********************************************************************************************"
      - Finds in a tuple an element that satisfies the given predicate and returns the tuple index. It returns an index beyond the last element if no element satisfies the predicate.
  defined_in_file: concurrency/tuple_algorithm.hpp
  overloads:
    "template <typename T, typename Op>\nstd::size_t tuple_find(const T &, Op)":
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
      signature_with_names: "template <typename T, typename Op>\nstd::size_t tuple_find(const T & t, Op op)"
  namespace:
    - stlab
    - v1
---
