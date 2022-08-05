---
layout: method
title: stlab::main_executor
tags: [library]
pure-name: operator()
defined-in-header: stlab/concurrency/main_executor.hpp 
description: Executes the given function on the application's main loop.
entities:
  - kind: methods
    list:
      - name: stlab::main_executor::operator()
        pure-name: operator()
        defined-in-header: stlab/concurrency/main_executor.hpp 
        declaration: |
          template <typename F> 
          void operator()(F f) const
        description: This executors executes the given function object on the applications main loop.
  - kind: parameters
    list:
      - name: f
        description: The function object that shall be executed on the main loop.
---
