---
layout: function
title: async
owner: sean-parent
brief: Creates a future running on a given executor
tags:
  - function
defined_in_file: concurrency/future.hpp
overloads:
  "template <typename E, typename F, typename... Args>\nauto async(E, F &&, Args &&...) -> future<detail::result_t<std::decay_t<F>, std::decay_t<Args>...>>":
    arguments:
      - description: The executor that shall be used to run the given callable f.
        name: executor
        type: E
      - description: The callable object
        name: f
        type: F &&
      - description: All arguments that are needed to call f, if needed.
        name: args
        type: Args &&...
    description: The template function creates future that runs the callable object f on the specified executor.
    return: The future on the for the underlying task f
    signature_with_names: "template <typename E, typename F, typename... Args>\nauto async(E executor, F && f, Args &&... args) -> future<detail::result_t<std::decay_t<F>, std::decay_t<Args>...>>"
namespace:
  - stlab
  - v1
---
