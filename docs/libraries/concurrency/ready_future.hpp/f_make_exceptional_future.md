---
layout: function
title: make_exceptional_future
owner: sean-parent
brief: Creates a future that is fulfilled as failed
tags:
  - function
defined_in_file: concurrency/ready_future.hpp
overloads:
  "template <typename T, typename E>\nfuture<T> make_exceptional_future(std::exception_ptr, E)":
    arguments:
      - description: The exception pointer
        name: error
        type: std::exception_ptr
      - description: The executor that will be used for an attached continuation
        name: executor
        type: E
    description: This function creates a future that is fulfilled as failed.
    return: A future that is ready in an error state.
    signature_with_names: "template <typename T, typename E>\nfuture<T> make_exceptional_future(std::exception_ptr error, E executor)"
namespace:
  - stlab
  - v1
---
Creates a future that is fulfilled as failed
