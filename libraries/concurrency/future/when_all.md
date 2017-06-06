---
layout: free-function
title: stlab::when_all
tags: [library]
pure-name: when_all
brief: Creates a joining future
description: Creates a joining future
annotation: template function
example: when_all_example.cpp
entities:
  - kind: overloads
    defined-in-header: stlab/concurrency/future.hpp
    git-link: https://github.com/stlab/libraries/blob/develop/stlab/concurrency/future.hpp
    list:
      - name: when_all
        pure-name: when_all
        declaration: |
            template <typename E, typename F, typename...Ts>
            auto when_all(E e, F f, future<Ts>... args)
        description: This function create a joining future. When all passed `args` futures are fullfilled, then the continuation tasks defined with `f` is scheduled on the executor `e`.
      - name: when_all
        pure-name: when_all
        declaration: |
            template <typename E, typename F, typename I>
            auto when_all(E e, F f, const std::pair<I,I>& range)
        description: This function create a joining future
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
