---
layout: method
title: stlab::task::target
tags: [library]
scope: task
pure-name: target
defined-in-header: stlab/concurrency/task.hpp
brief: Get the underlying function
entities:
  - kind: methods
    list:
      - name: stlab::task::target
        pure-name: target
        defined-in-header: stlab/concurrency/task.hpp 
        declaration: |
            template <class T>
            T* target()
        description: Nonconst variant
      - name: stlab::task::target
        pure-name: target
        defined-in-header: stlab/concurrency/task.hpp 
        declaration: |
            template <class T>
            const T* target() const
        description: Const variant
  - kind: result
    description: |
        A pointer to the underlying function, if `target_type() == typeid(T)`. Otherwise, `nullptr`.
---
