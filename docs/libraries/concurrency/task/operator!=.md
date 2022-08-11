---
layout: function
title: stlab::operator!=
tags: [library]
scope: stlab
pure-name: operator!=
brief: Compares a task for not equal an empty task
description: Compares a task for not equal an empty task
defined-in-header: stlab/concurrency/task.hpp
entities:
  - kind: overloads
    list:
      - name: operator!=
        pure-name: operator!=
        declaration: |
            bool operator!=(const task& x, std::nullptr_t)
        description: Checks if the given task `x` is not equal an empty task
      - name: operator!=
        pure-name: when_all
        declaration: |
            bool operator!=(std::nullptr_t, const task& x)
        description: Checks if the given task `x` is not equal an empty task
  - kind: parameters
    list:
      - name: x
        description: The task that shall be compared against an empty task
  - kind: result
    description: Returns true, if the given task is not equal an empty task.
---
