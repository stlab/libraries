---
layout: function
title: merge_channel
owner: sean-parent
brief: Creates a channel that merges all passed arguments
tags:
  - function
defined_in_file: concurrency/channel.hpp
overloads:
  "template <typename M, typename S, typename F, typename... R>\nauto merge_channel(S, F, R...)":
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
    description: Creates a receiver with an attached process that executes the by `M`specified merge strategy whenever an upstream process provides a value. `M` can be of type `round_robin_t`, `unordered_t`, or `zip_with`.
    return: a receiver that merges all passed arguments
    signature_with_names: "template <typename M, typename S, typename F, typename... R>\nauto merge_channel(S s, F f, R... upstream_receiver)"
namespace:
  - stlab
  - v1
---
