---
layout: method
title: stlab::default_executor
tags: [library]
pure-name: operator()
defined-in-header: stlab/concurrency/default_executor.hpp 
declaration: operator()
description: Executes the given function on the operating systems's thread pool.
entities:
  - kind: methods
    list:
      - name: stlab::default_executor::operator()
        pure-name: operator()
        defined-in-header: stlab/concurrency/default_executor.hpp 
        declaration: |
          template <typename F> 
          void operator()(F&& f) const
        description: Pushes the given function object on the systems thread pool.
  - kind: parameters
    list:
      - name: f
        description: The function object that shall be executed on the thread pool.
---
