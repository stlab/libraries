---
layout: function
title: zip
owner: sean-parent
brief: Zips the values in step from all upstream receivers. (The functionality has changed after release 1.2.0!)
tags:
  - function
defined_in_file: concurrency/channel.hpp
overloads:
  "template <typename S, typename... R>\nauto zip(S, R...)":
    arguments:
      - description: Executor which is used to schedule the resulting task
        name: s
        type: S
      - description: The upstream receiver(s).
        name: r
        type: R...
    description: Creates a receiver of type `tuple<T...>` where `T...` are the `result_type`s of the passed `upstream_receiver`. Whenever a complete set of values from each upstream receiver has arrived, it passes the tuple with the values downstream.
    return: a `tuple<T...>` where `T...` are the `result_types` of all `upstream_receiver`.
    signature_with_names: "template <typename S, typename... R>\nauto zip(S s, R... r)"
namespace:
  - stlab
  - v1
---
