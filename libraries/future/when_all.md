---
layout: free-function
title: stlab::when_all
tags: [library]
entities:
  - kind: overloads
    defined-in-header: stlab/future.hpp
    list:
      - name: when_all
        pure-name: when_all
        declaration: |
            template <typename E, typename F, typename...Ts>
            auto when_all(E e, F f, future<Ts>... args)
        description: This function create a joining future. When all passed args futures are fullfilled, then the continuation tasks defined with f is scheduled on the executor e.
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
  - kind: result
    description: a future that joins all passed arguments and passes them to the associated function object
---
