---
layout: function
title: stlab::merge_channel
tags: [library]
scope: stlab
pure-name: merge_channel
brief: Creates a channel that merges all passed arguments
annotation: template function
example: merge_channel_example.cpp
defined-in-header: stlab/concurrency/channel.hpp 
entities:
  - kind: overloads
    list:
      - name: merge_channel
        pure-name: merge_channel
        declaration: |
            template <typename M, typename E, typename F, typename...R>
            auto merge_channel(E e, F f, R... upstream_receiver)
        description: This function creates a receiver with an attached process that executes the by `M`specified merge strategy whenever an upstream process provides a value. `M` can be of type `round_robin_t`, `unordered_t`, or `zip_with`.
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
