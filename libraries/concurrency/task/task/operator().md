---
layout: method
title: stlab::task::operator()
tags: [library]
scope: task
pure-name: operator()
defined-in-header: stlab/concurrency/task.hpp
brief: Invoke the task
entities:
  - kind: methods
    list:
      - name: stlab::task::operator()
        pure-name: operator()
        defined-in-header: stlab/concurrency/task.hpp 
        declaration: |
            void operator()(Args... args) noexcept
        description: Invokes the task
  - kind: parameters
    list:
      - name: args
        description: The arguments for calling the stored function.

---
