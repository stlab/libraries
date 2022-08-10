---
layout: method
title: operator|
owner: sean-parent
brief: Creates a continuation on the current future.
example: [then_continuation_example.cpp, then_split_example.cpp]
tags:
  - method
defined_in_file: concurrency/future.hpp
overloads:
  "template <typename F>\nauto operator|(F &&) &&":
    arguments:
      - description: __OPTIONAL__
        name: f
        type: F &&
    description: Constructs a continuation with the passed callable object using the provided executor.
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\nauto operator|(F && f) &&"
  "template <typename F>\nauto operator|(F &&) const &":
    arguments:
      - description: __OPTIONAL__
        name: f
        type: F &&
    description: Constructs a continuation on a r-value with the passed callable object using the same.
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\nauto operator|(F && f) const &"
  "template <typename F>\nauto operator|(executor_task_pair<F>) &&":
    arguments:
      - description: __OPTIONAL__
        name: etp
        type: executor_task_pair<F>
    description: Constructs a continuation on a r-value with the passed callable object using the provided executor.
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\nauto operator|(executor_task_pair<F> etp) &&"
  "template <typename F>\nauto operator|(executor_task_pair<F>) const &":
    arguments:
      - description: __OPTIONAL__
        name: etp
        type: executor_task_pair<F>
    description: Constructs a continuation with the passed callable object using the provided executor.
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\nauto operator|(executor_task_pair<F> etp) const &"
---
Creates a new continuation on the current object. 

Multiple `operator|()` invocations on the same future object realize a split. The result of this will be copied into all continuations. 

If `T` of `future<T>` is a move only type, then a split is not possible of course.
