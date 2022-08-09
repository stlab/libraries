---
layout: method
title: operator^
owner: sean-parent
brief: Creates a recoverable future on the current object.
example: recover_example.cpp
tags:
  - method
defined_in_file: concurrency/future.hpp
overloads:
  "template <typename F>\nauto operator^(F &&) &&":
    arguments:
      - description: Callable object that implements the continuation. `T` of this must be convertible to the parameter of `f`.
        name: f
        type: F &&
    description: Constructs a recoverable continuation on an r-value future. It gets a `future<T>` passed.
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\nauto operator^(F && f) &&"
  "template <typename F>\nauto operator^(F &&) const &":
    arguments:
      - description: Callable object that implements the continuation. `T` of this must be convertible to the parameter of `f`.
        name: f
        type: F &&
    description: Constructs a recoverable continuation that gets a `future<T>` passed.
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\nauto operator^(F && f) const &"
  "template <typename F>\nauto operator^(executor_task_pair<F>) &&":
    arguments:
      - description: __OPTIONAL__
        name: etp
        type: executor_task_pair<F>
    description: Constructs a recoverable continuation that gets a `future<T>` passed. It uses the provided executor.
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\nauto operator^(executor_task_pair<F> etp) &&"
  "template <typename F>\nauto operator^(executor_task_pair<F>) const &":
    arguments:
      - description: Callable object that implements the continuation. `T` of this must be convertible to the parameter of `f`.
        name: etp
        type: executor_task_pair<F>
    description: Constructs a recoverable continuation that gets a `future<T>` passed. It uses the provided executor.
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\nauto operator^(executor_task_pair<F> etp) const &"
---
Since the futures are value based, the recover method should be used to do the error handling. Its functionality is identical to `.recover()`.


