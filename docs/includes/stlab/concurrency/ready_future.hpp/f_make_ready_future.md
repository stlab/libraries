---
layout: function
title: make_ready_future
hyde:
  owner: sean-parent
  brief: Creates a future that is already fulfilled
  tags:
    - function
  inline:
    brief:
      - "***********************************************************************************************"
  defined_in_file: stlab/concurrency/ready_future.hpp
  overloads:
    "template <typename E>\nauto make_ready_future(E) -> future<void>":
      arguments:
        - description: __OPTIONAL__
          name: executor
          type: E
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: "template <typename E>\nauto make_ready_future(E executor) -> future<void>"
    "template <typename T, typename E>\nauto make_ready_future(T &&, E) -> future<std::decay_t<T>>":
      arguments:
        - description: __OPTIONAL__
          name: x
          type: T &&
        - description: __OPTIONAL__
          name: executor
          type: E
      description: __INLINED__
      inline:
        description:
          - "***********************************************************************************************"
      return: __OPTIONAL__
      signature_with_names: "template <typename T, typename E>\nauto make_ready_future(T && x, E executor) -> future<std::decay_t<T>>"
  namespace:
    - stlab
    - v3
---

Creates a future that is already fulfilled
