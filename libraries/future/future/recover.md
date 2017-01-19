---
layout: method
title: stlab::future
tags: [library]
full-name: stlab::future::recover
pure-name: recover
defined-in-header: stlab/future.hpp 
declaration: recover()
description: Creates a recoverable continuation on the current object.
entities:
  - kind: methods
    list:
      - name: stlab::future::recover
        pure-name: recover
        defined-in-header: stlab/future.hpp 
        declaration: |
          template <typename F> 
          auto recover(F&& f) const&
        description: Constructs a continuation using the same scheduler as this.
      - name: stlab::future::recover
        pure-name: recover
        defined-in-header: stlab/future.hpp 
        declaration: |
          template <typename E, typename F> 
          auto recover(E&& e, F&& f) const&
        description: Constructs a continuation using the provided scheduler.
      - name: stlab::future::recover
        pure-name: recover
        defined-in-header: stlab/future.hpp 
        declaration: |
          template <typename F>
          auto recover(F&& f) &&
        description: Constructs a continuation using the same scheduler as this.
      - name: stlab::future::recover
        pure-name: recover
        defined-in-header: stlab/future.hpp 
        declaration: |
          template <typename E, typename F>
          auto recover(E&& e, F&& f) &&
        description: Constructs a continuation using the provided scheduler.
  - kind: parameters
    list:
      - name: e
        description: Executor which is used to schedule the resulting task
      - name: f
        description: Callable object that implements the recover function. Its parameter must be of type of this future instance.
  - kind: example
    code: NoCode
---
