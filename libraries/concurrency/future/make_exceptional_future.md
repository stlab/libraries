---
layout: function
title: stlab::make_exceptional_future
brief: Creates a future that is fulfilled as failed
defined-in-header: stlab/concurrency/future.hpp
tags:
  - function
overloads:
  "template <typename T, typename E>\nfuture<T> make_exceptional_future(std::exception_ptr, E)":
    description: This function creates a future that is fulfilled as failed.
    arguments:
      - description: The exception pointer
        name: exception
        type: std::exception_ptr
      - description: The executor that will be used for an attached continuation
        name: executor
        type: E
    return: A future that is ready in an error state.
    signature_with_names: "template <typename T, typename E>\nfuture<T> make_exceptional_future(std::exception_ptr exception, E executor)"
---
Creates a future that is fulfilled as failed
