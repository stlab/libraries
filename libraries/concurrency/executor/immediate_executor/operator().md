---
layout: method
title: stlab::immediate_executor
tags: [library]
pure-name: operator()
defined-in-header: stlab/concurrency/immediate_executor.hpp 
declaration: operator()
description: Executes the given function on this executor
entities:
  - kind: methods
    list:
      - name: stlab::immediate_executor::operator()
        pure-name: operator()
        defined-in-header: stlab/concurrency/immediate_executor.hpp 
        declaration: |
          template <typename F> 
          void operator()(F f) const
        description: Execute the given task immediately.
  - kind: parameters
    list:
      - name: f
        description: The task that shall now be executed.
---
