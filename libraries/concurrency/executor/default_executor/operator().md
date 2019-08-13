---
layout: method
title: stlab::default_executor
tags: [library]
pure-name: operator()
defined-in-header: stlab/concurrency/default_executor.hpp 
declaration: operator()
description: Executes the given task on the operating systems' thread pool.
entities:
  - kind: methods
    list:
      - name: stlab::high_executor::operator()
        pure-name: operator()
        defined-in-header: stlab/concurrency/default_executor.hpp 
        declaration: |
          template <typename F> 
          void operator()(F&& f) const
        description: Pushes the given task to the systems thread pool and it is executed with high priority.
      - name: stlab::default_executor::operator()
        pure-name: operator()
        defined-in-header: stlab/concurrency/default_executor.hpp 
        declaration: |
          template <typename F> 
          void operator()(F&& f) const
        description: Pushes the given task to the systems thread pool and it is executed with medium priority.
      - name: stlab::low_executor::operator()
        pure-name: operator()
        defined-in-header: stlab/concurrency/default_executor.hpp 
        declaration: |
          template <typename F> 
          void operator()(F&& f) const
        description: Pushes the given task to the systems thread pool and it is executed with low priority.
  - kind: parameters
    list:
      - name: f
        description: The task that shall be executed on the thread pool.
---
