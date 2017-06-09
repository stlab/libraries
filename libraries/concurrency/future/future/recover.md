---
layout: method
title: recover
tags: [library]
scope: future
pure-name: recover
defined-in-header: stlab/concurrency/future.hpp 
declaration: recover()
brief: Creates a recoverable future on the current object.
description: Since the futures are value based, the recover method should be used to do the error handling.    
example: recover_example.cpp
entities:
  - kind: methods
    list:
      - name: stlab::future::recover
        pure-name: recover
        defined-in-header: stlab/concurrency/future.hpp 
        declaration: |
          template <typename F> 
          auto recover(F&& f) const&
        description: Constructs a recoverable continuation that gets a `future<T>` passed. It uses the same executor as this.
      - name: stlab::future::recover
        pure-name: recover
        defined-in-header: stlab/concurrency/future.hpp 
        declaration: |
          template <typename E, typename F> 
          auto recover(E&& e, F&& f) const&
        description: Constructs a recoverable continuation that gets a `future<T>` passed. It uses the provided executor.
      - name: stlab::future::recover
        pure-name: recover
        defined-in-header: stlab/concurrency/future.hpp 
        declaration: |
          template <typename F>
          auto recover(F&& f) &&
        description: Constructs a recoverable continuation on an r-value future. It gets a `future<T>` passed. It uses the same executor as this.
      - name: stlab::future::recover
        pure-name: recover
        defined-in-header: stlab/concurrency/future.hpp 
        declaration: |
          template <typename E, typename F>
          auto recover(E&& e, F&& f) &&
        description: Constructs a recoverable continuation on an r-value future. It gets a `future<T>` passed. It uses the the provided executor.
  - kind: parameters
    list:
      - name: e
        description: Executor which is used to schedule the resulting task
      - name: f
        description: Callable object that implements the `recover()` function. Its parameter must be of type of this `future<T>` instance.
      - kind: result
        description: It returns the recoverable future

---
