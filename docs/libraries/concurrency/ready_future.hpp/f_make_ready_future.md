---
layout: function
title: make_ready_future
owner: sean-parent
brief: Creates a future that is already fulfilled
tags:
  - function
defined_in_file: concurrency/ready_future.hpp
overloads:
  "template <typename E>\nfuture<void> make_ready_future(E)":
    arguments:
      - description: The executor that will be used for an attached continuation
        name: executor
        type: E
    description: This function creates a future that is already fulfilled.
    return: __OPTIONAL__
    signature_with_names: "template <typename E>\nfuture<void> make_ready_future(E executor)"
  "template <typename T, typename E>\nfuture<std::decay_t<T>> make_ready_future(T &&, E)":
    arguments:
      - description: The result value of the future
        name: x
        type: T &&
      - description: The executor that will be used for an attached continuation
        name: executor
        type: E
    description: This function creates a future that is already fulfilled.
    return: __OPTIONAL__
    signature_with_names: "template <typename T, typename E>\nfuture<std::decay_t<T>> make_ready_future(T && x, E executor)"
namespace:
  - stlab
  - v1
---
Creates a future that is already fulfilled
