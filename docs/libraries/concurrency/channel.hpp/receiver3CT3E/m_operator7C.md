---
layout: method
title: operator|
owner: FelixPetriconi
brief: Attaches a new process to the channel.
example: operator_pipe_example.cpp
tags:
  - method
defined_in_file: concurrency/channel.hpp
overloads:
  auto operator|(sender<T>):
    arguments:
      - description: __OPTIONAL__
        name: send
        type: sender<T>
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: auto operator|(sender<T> send)
  "template <typename F>\nauto operator|(F &&) const":
    arguments:
      - description: __OPTIONAL__
        name: f
        type: F &&
    description: Attaches the given process to the receiver and returns a new receiver. It can either be a function object with a single parameter or a process that has an `await()` and a `yield()` method. L-value processes can be attached when they are wrapped within `std::reference_wrapper`.
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\nauto operator|(F && f) const"
  "template <typename F>\nauto operator|(detail::annotated_process<F>)":
    arguments:
      - description: The process to be executed.
        name: ap
        type: detail::annotated_process<F>
    description: Attaches the given annotated process to the receiver and returns a new receiver. It can either be a function object with a single parameter or a process that follows this signature. An `annotated_process` should not be explicitly instantiated but it gets implicitly created as soon as one combines a function object with an `operator&` and an `executor` or a `buffer_size` or both. L-value processes can be attached when they are wrapped within `std::reference_wrapper`.
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\nauto operator|(detail::annotated_process<F> ap)"
  "template <typename F>\nauto operator|(executor_task_pair<F>)":
    arguments:
      - description: __OPTIONAL__
        name: etp
        type: executor_task_pair<F>
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\nauto operator|(executor_task_pair<F> etp)"
---

`operator|` creates a new receiver, attaches the given process as downstream to it and returns this new receiver. The new receiver inherits the executor from its upstream receiver if not an alternative executor is attached. In case that `T` of `receiver<T>` is a move only type, repeated calls of this operator overwrite the previous attached downstream channel.