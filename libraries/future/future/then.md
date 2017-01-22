---
layout: method
title: stlab::future
tags: [library]
full-name: stlab::future::then
pure-name: then
defined-in-header: stlab/future.hpp 
declaration: then()
description: Creates a new continuation on the current object.
entities:
  - kind: methods
    list:
      - name: stlab::future::get_try
        pure-name: get_try
        defined-in-header: stlab/future.hpp 
        declaration: |
          template <typename F> 
          auto then(F&& f) const&
        description: Constructs a continuation using the same executor as this.
      - name: stlab::future::then
        pure-name: then
        defined-in-header: stlab/future.hpp 
        declaration: |
          template <typename E, typename F> 
          auto then(E&& e, F&& f) const&
        description: Constructs a continuation using the provided executor.
      - name: stlab::future::then
        pure-name: then
        defined-in-header: stlab/future.hpp 
        declaration: |
          template <typename F>
          auto then(F&& f) &&
        description: Constructs a continuation using the same executor as this.
      - name: stlab::future::then
        pure-name: then
        defined-in-header: stlab/future.hpp 
        declaration: |
          template <typename E, typename F>
          auto then(E&& e, F&& f) &&
        description: Constructs a continuation using the provided executor.
  - kind: parameters
    list:
      - name: e
        description: Executor which is used to schedule the resulting task
      - name: f
        description: Callable object that implements the recover function. Its parameter must be of type of this future instance.
  - kind: result
    description: a future of the type of the result type of the passed function object.
  - kind: example
    code: NoCode
---
