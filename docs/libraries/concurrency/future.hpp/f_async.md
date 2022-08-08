---
layout: function
title: stlab::async
brief: Creates a future running on a given executor
defined-in-header: stlab/concurrency/future.hpp
tags:
  - function
example: async_example.cpp
overloads:
  "template <typename E, typename F, typename... Args>\nfuture<std::result_of_t<F(Args...)>> async(E, F&&, Args&&... )":
    description: The template function creates future that runs the callable object f on the specified executor.
    arguments:
      - description: The executor that shall be used to run the given callable f.
        name: executor
        type: E
      - description: The callable object
        name: f
        type: F&&
      - description: All arguments that are needed to call f, if needed.
        name: args
        type: Args&&
    return: The future on the for the underlying task f
    signature_with_names: "template <typename E, typename F, typename... Args>\nfuture<std::result_of_t<F(Args...)>> async(E executor, F&& f, Args&&... args)"
---
