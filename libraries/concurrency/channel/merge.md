---
layout: function
title: stlab::merge
tags: [library]
scope: stlab
pure-name: merge
brief: Creates a receiver that merges all passed arguments. This function is deprecated and will be removed soon. Please instead `merge_channel<unordered_t>`.
annotation: template function
example: merge_channel_example.cpp
defined-in-header: stlab/concurrency/channel.hpp 
entities:
  - kind: overloads
    list:
      - name: merge
        pure-name: merge
        declaration: |
            template <typename E, typename F, typename...R>
            [[deprecated]] auto merge(E e, F f, R... upstream_receiver)
        description: This function creates a process that executes the provided function object whenever an upstream process provides a value. There is no defined order in which the process `f` is called with the incoming upstream values.
  - kind: parameters
    list:
      - name: e
        description: Executor which is used to schedule the resulting task
      - name: f
        description: Callable object that implements the process. All results from the upstream process must be convertible to the only argument of the provided function object's function operator or the argument of process' await function.
      - name: upstream_receiver
        description: The upstream receiver. 
  - kind: result
    description: a receiver that merges all passed arguments
---
