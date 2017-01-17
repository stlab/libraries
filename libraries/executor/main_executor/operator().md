---
layout: method
title: stlab::main_executor
tags: [library]
full-name: stlab::main_executor::operator()
pure-name: operator()
defined-in-header: stlab/main_executor.hpp 
declaration: operator()
description: Executes the given function on this executor
entities:
  - kind: methods
    list:
      - name: stlab::main_executor::operator()
        pure-name: operator()
        defined-in-header: stlab/main_executor.hpp 
        declaration: template <typename F> void operator()(F&& f) const
        description: This executors executes the given function object on the applications main loop.
  - kind: example
    code: NoCode
---
