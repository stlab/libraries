---
layout: method
title: stlab::task::operator=
tags: [library]
scope: task
pure-name: operator=
defined-in-header: stlab/concurrency/task.hpp
brief: Move assignment operator
entities:
  - kind: methods
    list:
      - name: stlab::task::operator=
        pure-name: operator=
        defined-in-header: stlab/concurrency/task.hpp 
        declaration: |
            task& operator=(task&& x) noexcept
        description: Moves the given task `x` into `this`.
      - name: stlab::task::operator=
        pure-name: operator=
        defined-in-header: stlab/concurrency/task.hpp 
        declaration: |
            task& operator=(std::nullptr_t) noexcept
        description: Destroys the current function and creates a new empty one.
  - kind: parameters
    list:
      - name: x
        description: The task that shall be moved into this.
  - kind: result
    description: |
        `*this`
---
