---
layout: function
title: operator|
brief: Creates a continuation on the current future.
defined-in-header: stlab/concurrency/future.hpp
tags:
  - function
example: [then_continuation_example.cpp, then_split_example.cpp]
overloads:
  "template <typename F>\nauto operator|(F&&)":
    description: Constructs a continuation with the passed callable object using the same executor as this.
    arguments:
      - description: |
          Callable object that implements the continuation. `T` of this must be convertible to the parameter of `f`.
        name: f
        type: F
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\nauto operator|(F&& f) const"
  "template <typename F>\nauto operator|(F&& f) &&":
    description: Constructs a continuation on a r-value with the passed callable object using the same. 
    arguments:
      - description: |
          Callable object that implements the continuation. `T` of this must be convertible to the parameter of `f`.
        name: x
        type: executor_task_pair<F>
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\nauto operator|(F&& f) &&"
  "template <typename F>\nauto operator|(executor_task_pair<F>)": 
    description: Constructs a continuation with the passed callable object using the provided executor.
    arguments:
      - description: |
          Callable object that implements the continuation. `T` of this must be convertible to the parameter of `f`.
        name: x
        type: executor_task_pair<F>
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\nauto operator|(executor_task_pair<F> x) const"
  "template <typename F>\nauto operator|(executor_task_pair<F>)&&": 
    description: Constructs a continuation on a r-value with the passed callable object using the provided executor.
    arguments:
      - description: |
          Callable object that implements the continuation. `T` of this must be convertible to the parameter of `f`.
        name: x
        type: executor_task_pair<F>
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\nauto operator|(executor_task_pair<F> x) &&"
---
Creates a new continuation on the current object. 

Multiple `operator|()` invocations on the same future object realize a split. The result of this will be copied into all continuations. 

If `T` of `future<T>` is a move only type, then a split is not possible of course.
