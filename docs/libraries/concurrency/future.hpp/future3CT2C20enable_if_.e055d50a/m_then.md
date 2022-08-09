---
layout: method
title: then
owner: sean-parent
brief: Creates a continuation on the current future.
example: [then_continuation_example.cpp, then_split_example.cpp]
tags:
  - method
defined_in_file: concurrency/future.hpp
overloads:
  "template <typename E, typename F>\nauto then(E &&, F &&) &&":
    arguments:
      - description: Executor which is used to schedule the resulting task.
        name: executor
        type: E &&
      - description: Callable object that implements the continuation. `T` of this must be convertible to the parameter of `f`.
        name: f
        type: F &&
    description: Constructs a continuation on an r-value with the passed callable object using the provided executor.
    return: a future of the type of the result type of the passed function object.
    signature_with_names: "template <typename E, typename F>\nauto then(E && executor, F && f) &&"
  "template <typename E, typename F>\nauto then(E &&, F &&) const &":
    arguments:
      - description: Executor which is used to schedule the resulting task.
        name: executor
        type: E &&
      - description: Callable object that implements the continuation. `T` of this must be convertible to the parameter of `f`.
        name: f
        type: F &&
    description: Constructs a continuation with the passed callable object using the provided executor.
    return: a future of the type of the result type of the passed function object.
    signature_with_names: "template <typename E, typename F>\nauto then(E && executor, F && f) const &"
  "template <typename F>\nauto then(F &&) &&":
    arguments:
      - description: Callable object that implements the continuation. `T` of this must be convertible to the parameter of `f`.
        name: f
        type: F &&
    description: Constructs a continuation on a r-value with the passed callable object using the same executor as this.
    return: a future of the type of the result type of the passed function object.
    signature_with_names: "template <typename F>\nauto then(F && f) &&"
  "template <typename F>\nauto then(F &&) const &":
    arguments:
      - description: Callable object that implements the continuation. `T` of this must be convertible to the parameter of `f`.
        name: f
        type: F &&
    description: Constructs a continuation with the passed callable object using the same executor as this.
    return: a future of the type of the result type of the passed function object.
    signature_with_names: "template <typename F>\nauto then(F && f) const &"
---
Creates a new continuation on the current object. 

Multiple `then()` invocations on the same future object realize a split. The result of this will be copied into all continuations. 

If `T` of `future<T>` is a move only type, then a split is not possible of course.