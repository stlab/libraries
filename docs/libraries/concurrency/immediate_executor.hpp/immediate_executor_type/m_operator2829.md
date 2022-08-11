---
layout: method
title: operator()
owner: sean-parent
brief: Call operator to schedule a task
tags:
  - method
defined_in_file: concurrency/immediate_executor.hpp
overloads:
  "template <typename F>\nvoid operator()(F &&) const":
    arguments:
      - description: __OPTIONAL__
        name: f
        type: F &&
    description: The task that shall now be executed.
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\nvoid operator()(F && f) const"
  "template <typename F>\nvoid operator()(std::chrono::steady_clock::time_point, F &&) const":
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: std::chrono::steady_clock::time_point
        unnamed: true
      - description: __OPTIONAL__
        name: f
        type: F &&
    description: Execute the given task immediately.
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\nvoid operator()(std::chrono::steady_clock::time_point, F && f) const"
---

Executes the given function on this executor