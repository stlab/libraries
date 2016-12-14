---
layout: free-function
title: stlab::when_all
tag: library
entities:
  - kind: overloads
    list:
      - name: when_all
        pure-name: when_all
        defined-in-header: stlab/future.hpp
        declaration: |
            template <typename S, typename F, typename...Ts>
            auto when_all(S s, F f, future<Ts>... args);
        description: This function create a joining future. When all passed args futures are fullfilled, then the continuation tasks defined with f is scheduled on the scheduler s.
      - name: when_all
        pure-name: when_all
        defined-in-header: stlab/future.hpp
        declaration: |
            template <typename S, typename F, typename I> 
            auto when_all(S s, F f, const std::pair<I,I>& range);
        description: This function create a joining future 
  - kind: parameters
    list:
      - name: s
        description: Scheduler which is used to schedule the resulting task
      - name: f
        description: Callable object that implements the task
      - name: args
        description: Futures that shall be joined
  - kind: result
    description: Returns a future that joins all passed arguments
---
