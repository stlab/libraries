---
layout: free-function
title: stlab::when_any
tags: [library]
scope: stlab
pure-name: when_any
brief: Creates a future that continues on the first success of any futures passed
description: Creates a future that continues on the first success of any futures passed. The results of all passed futures must be convertible to the first parameter of the continuing function. The function must accept the a second parameter of std::size_t. It will receive the index of the future that succeeded first.
annotation: template function
example: [ when_any_example.cpp, when_any_void_example.cpp ]
defined-in-header: stlab/concurrency/future.hpp
entities:
  - kind: overloads
    list:
      - name: when_any
        pure-name: when_any
        declaration: |
            template <typename E, typename F, typename...Ts>
            auto when_any(E e, F f, future<Ts>... args)
        description: This function create a future that continues whenever the first future of the passed arguments `args` succeeds.
      - name: when_any
        pure-name: when_any
        declaration: |
            template <typename E, typename F, typename I> 
            auto when_any(E e, F f, const std::pair<I,I>& range)
        description: This function create a future that continues whenever the first future within the range succeeds. In case that it is a range of futures of move-only types, the futures are moved internally out of the range into the function.
  - kind: parameters
    list:
      - name: e
        description: Executor which is used to schedule the resulting task
      - name: f
        description: Callable object that implements the task
      - name: args
        description: Futures that shall be joined
      - name: range
        description: This describes the range of futures. If an empty range is provided then an stlab::future_exception with code stlab::future_errc::broken_promise is thrown.
  - kind: result
    description: a future that reduces a number of input futures to the first that successds and passes it to the associated function object
---
