---
layout: function
title: stlab::when_any
brief: Creates a future that continues on the first success of any futures passed
defined-in-header: stlab/concurrency/future.hpp
tags:
  - function
example: [ when_any_example.cpp, when_any_void_example.cpp ]
overloads:
  "template <typename E, typename F, typename...Ts>\nauto when_any(E, F, future<Ts>...)":
    description: This function create a future that continues whenever the first future of the passed arguments `args` succeeds.
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
    signature_with_names: "template <typename E, typename F, typename...Ts>\nauto when_any(E executor, F f, future<Ts>... args)"
  "template <typename E, typename F, typename I>\nauto when_any(E, F, std::pair<I,I>)":
    description: This function create a future that continues whenever the first future within the range succeeds. In case that it is a range of futures of move-only types, the futures are moved internally out of the range into the function.
    arguments:
      - description: Executor which is used to schedule the resulting task
        name: executor
        type: E
      - description: Callable object that implements the continuing task
        name: f
        type: F
      - description: This describes the range of futures. If an empty range is provided then an stlab::future_exception with code stlab::future_errc::broken_promise is thrown.
        name: range
        type: std::pair<I,I>
    return: The continuation on the group of passed futures.
    signature_with_names: "template <typename E, typename F, typename I>\nauto when_all(E executor, F f, std::pair<I,I> range)"
---
Creates a future that continues on the first success of any futures passed. The results of all passed futures must be convertible to the first parameter of the continuing function. The function must accept the a second parameter of std::size_t. It will receive the index of the future that succeeded first.
