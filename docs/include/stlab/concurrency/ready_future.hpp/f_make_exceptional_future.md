---
layout: function
title: make_exceptional_future
hyde:
  owner: sean-parent
  brief: Creates a future that is fulfilled as failed
  tags:
    - function
  defined_in_file: stlab/concurrency/ready_future.hpp
  overloads:
    "template <typename T, typename E>\nauto make_exceptional_future(std::exception_ptr, E) -> future<T>":
      arguments:
        - description: __OPTIONAL__
          name: error
          type: std::exception_ptr
        - description: __OPTIONAL__
          name: executor
          type: E
      description: __OPTIONAL__
      return: __OPTIONAL__
      signature_with_names: "template <typename T, typename E>\nauto make_exceptional_future(std::exception_ptr error, E executor) -> future<T>"
  namespace:
    - stlab
    - v3
---

Creates a future that is fulfilled as failed
