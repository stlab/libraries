---
layout: method
title: stlab::task::task
tags: [library]
scope: task
pure-name: task
defined-in-header: stlab/concurrency/task.hpp
brief: Constructs a task
entities:
  - kind: methods
    list:
      - name: stlab::task::task
        pure-name: task
        defined-in-header: stlab/concurrency/task.hpp 
        declaration: |
            template <class F>
            task(F&& f)
        description: Moves the passed function `f` into the task
  - kind: parameters
    list:
      - name: f
        description: Callable object to be moved into the task.
---
