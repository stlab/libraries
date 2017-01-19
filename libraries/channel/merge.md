---
layout: free-function
title: stlab::merge
tags: [library]
entities:
  - kind: overloads
    defined-in-header: stlab/channel.hpp 
    git-link: https://github.com/stlab/libraries/blob/develop/stlab/channel.hpp 
    list:
      - name: merge
        pure-name: merge
        declaration: |
            template <typename E, typename F, typename...R>
            auto merge(E e, F f, R&&... upstream_receiver)
        description: This function creates a process that executes the provided function object whenever an upstream process provides a value.
  - kind: parameters
    list:
      - name: e
        description: Executor which is used to schedule the resulting task
      - name: f
        description: Callable object that implements the task. All results from the upstream process must be convertable to the only argument of the provided function object.
      - name: upstream_receiver
        description: The upstream receiver. 
  - kind: result
    description: a future that merges all passed arguments
---
