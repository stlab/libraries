---
layout: method
title: operator|
tags: [library]
pure-name: operator|
defined-in-header: stlab/concurrency/channel.hpp 
declaration: operator|()
brief: Creates a new receiver with the given process attached downstream.
description: Creates a new receiver, attaches the given process as downstream to it and returns this new receiver. The new receiver inherits the executor from its upstream receiver if not an alternative executor is attached. In case that `T` of `receiver<T>` is a move only type, repeated calls of this operator overwrite the previous attached downstream channel.
example: operator_pipe_example.cpp
entities:
  - kind: methods
    list:
      - name: stlab::receiver::operator|
        pure-name: operator|
        defined-in-header: stlab/concurrency/channel.hpp 
        declaration: |
          template <typename F> 
          auto operator|(F&& f)
        description: Attaches the given process to the receiver and returns a new receiver. It can either be a function object with a single parameter or a process that has an `await()` and a `yield()` method.
      - name: stlab::receiver::operator|
        pure-name: operator|
        defined-in-header: stlab/concurrency/channel.hpp 
        declaration: |
          template <typename F>
          auto operator|(detail::annotated_process<F>&& ap)
        description: Attaches the given annotated process to the receiver and returns a new receiver. It can either be a function object with a single parameter or a process that follows this signatur. An `annotated_process` should not be explicitly instantiated but it gets implicitly created as soon as one combines a function object with an `operator&` and an `executor` or a `buffer_size` or both.
  - kind: parameters
    list:
      - name: f
        description: The process to be executed.
      - name: args
        description: Futures that shall be joined
  - kind: result
    description: A receiver of type of the result of the passed function object
---
