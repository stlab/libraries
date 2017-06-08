---
layout: free-function
title: stlab::merge
tags: [library]
pure-name: merge
brief: Creates a future that merges all passed arguments
annotation: template function
example: merge_example.cpp
entities:
  - kind: overloads
    defined-in-header: stlab/concurrency/channel.hpp 
    git-link: https://github.com/stlab/libraries/blob/develop/stlab/concurrency/channel.hpp 
    list:
      - name: merge
        pure-name: merge
        declaration: |
            template <typename E, typename F, typename...R>
            auto merge(E e, F f, R&&... upstream_receiver)
        description: This function creates a process that executes the provided function object whenever an upstream process provides a value. There is no defined order in which the process `f` is called with the incoming upstream values.
  - kind: parameters
    list:
      - name: e
        description: Executor which is used to schedule the resulting task
      - name: f
        description: Callable object that implements the process. All results from the upstream process must be convertable to the only argument of the provided function object's function operator or the argument of process' await function.
      - name: upstream_receiver
        description: The upstream receiver. 
  - kind: result
    description: a future that merges all passed arguments
---
