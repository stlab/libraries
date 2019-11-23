---
layout: function
title: stlab::make_ready_future
brief: Creates a future that is already fulfilled
defined-in-header: stlab/concurrency/future.hpp
tags:
  - function
overloads:
  "template <typename T, typename E>\nfuture<T> make_ready_future(T&&, E)":
    description: This function creates a future that is already fulfilled.
    arguments:
      - description: The result value of the future
        name: x
        type: T&&
      - description: The executor that will be used for an attached continuation
        name: executor
        type: E
    signature_with_names: "template <typename T, typename E>\nfuture<T> make_ready_future(T&& x, E executor)"
  "template <typename E>\nfuture<void> make_ready_future(E)":
    description: This function creates a future that is already fulfilled.
    arguments:
      - description: The executor that will be used for an attached continuation
        name: executor
        type: E
    signature_with_names: "template <typename E>\nfuture<void> make_ready_future(E executor)"
---
Creates a future that is already fulfilled
