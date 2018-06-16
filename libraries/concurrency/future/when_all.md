---
layout: free-function
title: stlab::when_all
tags: [library]
scope: stlab
pure-name: when_all
brief: Creates a joining future
description: Creates a joining future
annotation: template function
example: [ when_all_example.cpp, when_all_void_example.cpp ]
defined-in-header: stlab/concurrency/future.hpp
entities:
  - kind: overloads
    list:
      - name: when_all
        pure-name: when_all
        declaration: |
            template <typename E, typename F, typename...Ts>
            auto when_all(E e, F f, future<Ts>... args)
        description: This function create a joining future. When all passed `args` futures are fulfilled, then the continuation tasks defined with `f` is scheduled on the executor `e`.
      - name: when_all
        pure-name: when_all
        declaration: |
            template <typename E, typename F, typename I>
            auto when_all(E e, F f, std::pair<I,I> range)
        description: This function create a joining future out of the given range. In case that it is a range of futures of move-only types, the futures are moved internally out of the range into the function.
  - kind: parameters
    list:
      - name: e
        description: Executor which is used to schedule the resulting task
      - name: f
        description: Callable object that implements the task
      - name: args
        description: Futures that shall be joined
      - name: range
        description: a range of futures of type `U`, specified by a half open range. All futures must succeed, before the continuation is triggered. It takes a `vector<U>` filled with all results as result.
  - kind: result
    description: a future that joins all passed arguments and passes them to the associated function object
---
