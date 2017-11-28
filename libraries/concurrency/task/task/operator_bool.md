---
layout: method
title: stlab::task::operator bool
tags: [library]
scope: task
pure-name: operator bool
defined-in-header: stlab/concurrency/task.hpp
brief: Check if the task can be invoked
entities:
  - kind: methods
    list:
      - name: stlab::task::operator bool
        pure-name: operator bool
        defined-in-header: stlab/concurrency/task.hpp 
        declaration: |
            explicit operator bool() const
        description: Used to determine if the task can be invoked
  - kind: result
    description: |
        `true` iff the task can be invoked.
---
