---
layout: method
title: stlab::task::swap
tags: [library]
scope: task
pure-name: swap
defined-in-header: stlab/concurrency/task.hpp
brief: Swaps an other task with this
entities:
  - kind: methods
    list:
      - name: stlab::task::swap
        pure-name: swap
        defined-in-header: stlab/concurrency/task.hpp 
        declaration: |
            void swap(task& x)
        description: Exchanges this task with the passed one.
  - kind: parameters
    list:
      - name: x
        description: The task that shall be exchanged with this
---
