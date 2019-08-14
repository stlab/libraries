---
layout: function
title: stlab::when_all
brief: Creates a joining future
defined-in-header: stlab/concurrency/future.hpp
tags:
  - function
example: [ when_all_example.cpp, when_all_void_example.cpp ]
overloads:
  "template <typename E, typename F, typename...Ts>\nauto when_all(E, F, future<Ts>...)":
    description: This function create a joining future. When all passed `args` futures are fulfilled, then the continuation tasks defined with `f` is scheduled on the executor `executor`.
    arguments:
      - description: Executor which is used to schedule the resulting task
        name: executor
        type: E
      - description: Callable object that implements the continuing task
        name: f
        type: F
      - description: Callable object that implements the continuing task
        name: args
        type: future<Ts>...
    return: The continuation on the group of passed futures.
    signature_with_names: "template <typename E, typename F, typename...Ts>\nauto when_all(E executor, F f, future<Ts>... args)"
  "template <typename E, typename F, typename I>\nauto when_all(E, F, std::pair<I,I>)":
    description: This function create a joining future out of the given range. In case that it is a range of futures of move-only types, the futures are moved internally out of the range into the function.
    arguments:
      - description: Executor which is used to schedule the resulting task
        name: executor
        type: E
      - description: Callable object that implements the continuing task
        name: f
        type: F
      - description: Callable object that implements the continuing task
        name: range
        type: std::pair<I,I>
    return: The continuation on the group of passed futures.
    signature_with_names: "template <typename E, typename F, typename I>\nauto when_all(E executor, F f, std::pair<I,I> range)"
---
