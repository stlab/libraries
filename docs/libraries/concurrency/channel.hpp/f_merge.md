---
layout: function
title: merge
owner: sean-parent
brief: Creates a receiver that merges all passed arguments. This function is deprecated and will be removed soon. Please instead `merge_channel<unordered_t>`.
tags:
  - function
defined_in_file: concurrency/channel.hpp
overloads:
  "template <typename S, typename F, typename... R>\nauto merge(S, F, R...)":
    annotation:
      - deprecated (Use merge_channel<unordered_t>)
    arguments:
      - description: Executor which is used to schedule the resulting task
        name: s
        type: S
      - description: Callable object that implements the process. All results from the upstream process must be convertible to the only argument of the provided function object's function operator or the argument of process' await function.
        name: f
        type: F
      - description: The upstream receiver.
        name: upstream_receiver
        type: R...
    description: Creates a process that executes the provided function object whenever an upstream process provides a value. There is no defined order in which the process `f` is called with the incoming upstream values.
    return: a receiver that merges all passed arguments
    signature_with_names: "template <typename S, typename F, typename... R>\nauto merge(S s, F f, R... upstream_receiver)"
namespace:
  - stlab
  - v1
---
