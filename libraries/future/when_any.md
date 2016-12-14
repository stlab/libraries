---
layout: free-function
title: stlab::when_any
entities:
  - kind: overloads
    list:
      - name: when_any
        pure-name: when_any
        defined-in-header: stlab/future.hpp
        declaration: |
            template <typename S, typename F, typename...Ts>
            auto when_any(S s, F f, future<Ts>... args);
        description: This function create a future that continues whenever the first future argument.
      - name: when_any
        pure-name: when_any
        defined-in-header: stlab/future.hpp
        declaration: |
            template <typename S, typename F, typename I> 
            auto when_any(S s, F f, const std::pair<I,I>& range);
        description: This function create a future that continues whenever the first future within the range succeeds
  - kind: parameters
    list:
      - name: s
        description: Scheduler which is used to schedule the resulting task
      - name: f
        description: Callable object that implements the task
      - name: args
        description: Futures that shall be joined
  - kind: result
    description: Creates a future that reduces a number of input futures to the first that successds
  - kind: example
    code: Fill in a code example
---