---
layout: method
title: then
tags: [library]
scope: future
pure-name: then
defined-in-header: stlab/concurrency/future.hpp
brief: Creates a continuation on the current future.
declaration: then()
example: [then_continuation_example.cpp, then_split_example.cpp]
description: |
  Creates a new continuation on the current object. 

  Multiple `then()` invocations on the same future object realize a split. The result of this will be copied into all continuations. 

  If `T` of `future<T>` is a move only type, then a split is not possible of course.
entities:
  - kind: methods
    list:
      - name: stlab::future::get_try
        pure-name: get_try
        defined-in-header: stlab/concurrency/future.hpp 
        declaration: |
          template <typename F> 
          auto then(F&& f) const&
        description: Constructs a continuation with the passed callable object using the same executor as this.
      - name: stlab::future::then
        pure-name: then
        defined-in-header: stlab/concurrency/future.hpp 
        declaration: |
          template <typename E, typename F> 
          auto then(E&& e, F&& f) const&
        description: Constructs a continuation with the passed callable object using the provided executor.
      - name: stlab::future::then
        pure-name: then
        defined-in-header: stlab/concurrency/future.hpp 
        declaration: |
          template <typename F>
          auto then(F&& f) &&
        description: Constructs a continuation on a r-value with the passed callable object using the same executor as this.
      - name: stlab::future::then
        pure-name: then
        defined-in-header: stlab/concurrency/future.hpp 
        declaration: |
          template <typename E, typename F>
          auto then(E&& e, F&& f) &&
        description: Constructs a continuation on a r-value with the passed callable object using the provided executor.
  - kind: parameters
    list:
      - name: e
        description: Executor which is used to schedule the resulting task
      - name: f
        description: Callable object that implements the continuation. `T` of this must be convertible to the parameter of `f`.
  - kind: result
    description: a future of the type of the result type of the passed function object.
  - kind: example
    code: NoCode
---
