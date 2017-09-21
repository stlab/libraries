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
            task() noexcept
        description: Default constructor for an empty task
      - name: stlab::task::task
        pure-name: task
        defined-in-header: stlab/concurrency/task.hpp 
        declaration: |
            task(task&&)
        description: Move constructor
      - name: stlab::task::task
        pure-name: task
        defined-in-header: stlab/concurrency/task.hpp 
        declaration: |
            task() noexcept
        description: Default constructor for an empty task        
      - name: stlab::task::task
        pure-name: task
        defined-in-header: stlab/concurrency/task.hpp 
        declaration: |
            template <class F>
            task(F&& f)
        description: Moves the passed function `f` into the task        
      - name: stlab::task::~task
        pure-name: task
        defined-in-header: stlab/concurrency/task.hpp 
        declaration: |
            ~task()
        description: Destructs the task. A not executed function is not executed.
  - kind: parameters
    list:
      - name: f
        description: Callable object to be moved into the task.
---
