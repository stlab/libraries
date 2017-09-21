---
layout: free-function
title: stlab::swap
tags: [library]
scope: stlab
pure-name: swap
brief: Exchanges two tasks
description: Exchanges two task
defined-in-header: stlab/concurrency/task.hpp
entities:
  - kind: overloads
    list:
      - name: swap
        pure-name: swap
        declaration: |
            bool swap(const task& x, std::nullptr_t)
        description: Checks if the given task `x` is not equal an empty task
  - kind: parameters
    list:
      - name: x
        description: The one task that shall be exchanged
      - name: y
        description: The other task that shall be exchanged
---
