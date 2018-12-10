---
layout: method
title: operator^()
tags: [library]
scope: future
pure-name: recover
defined-in-header: stlab/concurrency/future.hpp 
declaration: recover()
brief: Creates a recoverable future on the current object.
description: Since the futures are value based, the recover method should be used to do the error handling. Its functionality is identical to `.recover()`.    
example: recover_example.cpp
entities:
  - kind: methods
    list:
      - name: stlab::future::operator^
        pure-name: operator^
        defined-in-header: stlab/concurrency/future.hpp 
        declaration: |
          template <typename F> 
          auto operator^(F&& f) const&
        description: Constructs a recoverable continuation that gets a `future<T>` passed. It uses the same executor as this.
      - name: stlab::future::operator^
        pure-name: operator^
        defined-in-header: stlab/concurrency/future.hpp 
        declaration: |
          template <typename E, typename F> 
          auto operator^(executor_task_pair<F> etp) const&
        description: Constructs a recoverable continuation that gets a `future<T>` passed. It uses the provided executor.
      - name: stlab::future::operator^
        pure-name: operator^
        defined-in-header: stlab/concurrency/future.hpp 
        declaration: |
          template <typename F>
          auto operator^(F&& f) &&
        description: Constructs a recoverable continuation on an r-value future. It gets a `future<T>` passed. It uses the same executor as this.
      - name: stlab::future::operator^
        pure-name: operator^
        defined-in-header: stlab/concurrency/future.hpp 
        declaration: |
          template <typename E, typename F>
          auto operator^(executor_task_pair<F> etp) &&
        description: Constructs a recoverable continuation on an r-value future. It gets a `future<T>` passed. It uses the the provided executor.
  - kind: parameters
    list:
      - name: etp
        description: a wrapped pair of executor and task
      - name: f
        description: Callable object that implements the `recover()` function. Its parameter must be of type of this `future<T>` instance.
      - kind: result
        description: It returns the recoverable future

---
