---
layout: function
title: recover
brief: Creates a recoverable future on the current object.
defined-in-header: stlab/concurrency/future.hpp 
tags:
  - function  
example: recover_example.cpp
overloads:
  "template <typename F> \nauto recover(F&& f)":
    description: Constructs a recoverable continuation that gets a `future<T>` passed. It uses the same executor as this.
    arguments:
      - description: Callable object that implements the `recover()` function. Its parameter must be of type of this `future<T>` instance.
        name: f
        type: F
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\nauto recover(F&& f) const"
  "template <typename F> \nauto recover(F&& f)&&":
    description: Constructs a recoverable continuation on an r-value future. It gets a `future<T>` passed. It uses the same executor as this.
    arguments:
      - description: Callable object that implements the `recover()` function. Its parameter must be of type of this `future<T>` instance.
        name: f
        type: F
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\nauto recover(F&& f) &&" 
  "template <typename E, typename F> \nauto recover(E executor, F&& f)":
    description: Constructs a recoverable continuation that gets a `future<T>` passed. It uses the provided executor.
    arguments:
      - description: Executor which is used to schedule the resulting task
        name: executor
        type: E
      - description: Callable object that implements the `recover()` function. Its parameter must be of type of this `future<T>` instance.
        name: f
        type: F
    return: __OPTIONAL__
    signature_with_names: "template <typename E, typename F>\nauto recover(E executor, F&& f) const"
  "template <typename E, typename F> \nauto recover(E executor, F&& f)&&":
    description: Constructs a recoverable continuation on an r-value future. It gets a `future<T>` passed. It uses the the provided executor.
    arguments:
      - description: Executor which is used to schedule the resulting task
        name: executor
        type: E
      - description: Callable object that implements the `recover()` function. Its parameter must be of type of this `future<T>` instance.
        name: f
        type: F
    return: __OPTIONAL__
    signature_with_names: "template <typename E, typename F>\nauto recover(E executor, F&& f) &&"
---
Since the futures are value based, the recover method should be used to do the error handling.
