---
layout: function
title: operator^
brief: Creates a recoverable future on the current object.
defined-in-header: stlab/concurrency/future.hpp
tags:
  - function
example: recover_example.cpp
overloads:
  "template <typename F>\nauto operator^(F&& f)":
    description: Constructs a recoverable continuation that gets a `future<T>` passed.
    arguments:
      - description: Callable object that implements the continuation. `T` of this must be convertible to the parameter of `f`.
        name: f
        type: F
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\nauto operator^(F&& f) const"
  "template <typename F>\nauto operator^(F&& f)&&":
    description: Constructs a recoverable continuation that gets a `future<T>` passed.
    arguments:
      - description: Constructs a recoverable continuation on an r-value future. It gets a `future<T>` passed.
        name: f
        type: F
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\nauto operator^(F&& f) &&"
  "template <typename F>\nauto operator^(executor_task_pair<F>)": 
    description: Constructs a recoverable continuation that gets a `future<T>` passed. It uses the provided executor.
    arguments:
      - description: Callable object that implements the continuation. `T` of this must be convertible to the parameter of `f`.
        name: x
        type: executor_task_pair<F>
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\nauto operator^(executor_task_pair<F> x) const"
  "template <typename F>\nauto operator^(executor_task_pair<F>)&&": 
    description: Constructs a continuation on a r-value with the passed callable object using the provided executor.
    arguments:
      - description: Callable object that implements the continuation. `T` of this must be convertible to the parameter of `f`.
        name: x
        type: executor_task_pair<F>
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\nauto operator^(executor_task_pair<F> x) &&"
---
Since the futures are value based, the recover method should be used to do the error handling. Its functionality is identical to `.recover()`.


