---
layout: method
title: recover
owner: sean-parent
brief: Creates a recoverable future on the current object.
tags:
  - method
defined_in_file: concurrency/future.hpp
overloads:
  "template <typename E, typename F>\nauto recover(E &&, F &&) &&":
    arguments:
      - description: Executor which is used to schedule the resulting task
        name: executor
        type: E &&
      - description: Callable object that implements the `recover()` function. Its parameter must be of type of this `future<T>` instance.
        name: f
        type: F &&
    description: Constructs a recoverable continuation on an r-value future. It gets a `future<T>` passed. It uses the the provided executor.
    return: __OPTIONAL__
    signature_with_names: "template <typename E, typename F>\nauto recover(E && executor, F && f) &&"
  "template <typename E, typename F>\nauto recover(E &&, F &&) const &":
    arguments:
      - description: Executor which is used to schedule the resulting task
        name: executor
        type: E &&
      - description: Callable object that implements the `recover()` function. Its parameter must be of type of this `future<T>` instance.
        name: f
        type: F &&
    description: Constructs a recoverable continuation that gets a `future<T>` passed. It uses the provided executor.
    return: __OPTIONAL__
    signature_with_names: "template <typename E, typename F>\nauto recover(E && executor, F && f) const &"
  "template <typename F>\nauto recover(F &&) &&":
    arguments:
      - description: Callable object that implements the `recover()` function. Its parameter must be of type of this `future<T>` instance.
        name: f
        type: F &&
    description: Constructs a recoverable continuation on an r-value future. It gets a `future<T>` passed. It uses the same executor as this.
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\nauto recover(F && f) &&"
  "template <typename F>\nauto recover(F &&) const &":
    arguments:
      - description: Callable object that implements the `recover()` function. Its parameter must be of type of this `future<T>` instance.
        name: f
        type: F &&
    description: Constructs a recoverable continuation that gets a `future<T>` passed. It uses the same executor as this.
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\nauto recover(F && f) const &"
---
Since the futures are value based, the recover method should be used to do the error handling.
