---
layout: function
title: then
brief: Creates a continuation on the current future.
defined-in-header: stlab/concurrency/future.hpp
tags:
  - function
example: [then_continuation_example.cpp, then_split_example.cpp]
overloads:
  "template <typename F>\nauto then(F&&) const":
    description: Constructs a continuation with the passed callable object using the same executor as this.
    arguments:
      - description: Callable object that implements the continuation. `T` of this must be convertible to the parameter of `f`.
        name: f
        type: F
    return:  a future of the type of the result type of the passed function object.
    signature_with_names: "template <typename F>\nauto then(F&&) const"
  "template <typename F>\nauto then(F&&) &&":
    description: Constructs a continuation on a r-value with the passed callable object using the same executor as this.
    arguments:
      - description: Callable object that implements the continuation. `T` of this must be convertible to the parameter of `f`.
        name: f
        type: F
    return:  a future of the type of the result type of the passed function object.
    signature_with_names: "template <typename F>\nauto then(F&&) &&"
  "template <typename E, typename F>\nauto then(E, F&&) const":
    description: Constructs a continuation with the passed callable object using the same executor as this.
    arguments:
      - description: Executor which is used to schedule the resulting task.
        name: executor
        type: E    
      - description: Callable object that implements the continuation. `T` of this must be convertible to the parameter of `f`.
        name: f
        type: F
    return:  a future of the type of the result type of the passed function object.
    signature_with_names: "template <typename E, typename F>\nauto then(E executor, F&& f) const"
  "template <typename E, typename F>\nauto then(E, F&&) &&":
    description: Constructs a continuation on a r-value with the passed callable object using the same executor as this.
    arguments:
      - description: Executor which is used to schedule the resulting task.
        name: executor
        type: E        
      - description: Callable object that implements the continuation. `T` of this must be convertible to the parameter of `f`.
        name: f
        type: F
    return:  a future of the type of the result type of the passed function object.
    signature_with_names: "template <typename E, typename F>\nauto then(E executor, F&& f) &&"
---
Creates a new continuation on the current object. 

Multiple `then()` invocations on the same future object realize a split. The result of this will be copied into all continuations. 

If `T` of `future<T>` is a move only type, then a split is not possible of course.