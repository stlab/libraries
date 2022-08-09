---
layout: method
title: operator()
owner: sean-parent
brief: Passes the task to the serial_queue for scheduling and execution
tags:
  - method
defined_in_file: concurrency/serial_queue.hpp
overloads:
  "template <typename F, typename... Args>\nauto operator()(F &&, Args &&...) const":
    arguments:
      - description: __OPTIONAL__
        name: f
        type: F &&
      - description: __OPTIONAL__
        name: args
        type: Args &&...
    description: Returns a future that will contain the result of the executed task.
    return: __OPTIONAL__
    signature_with_names: "template <typename F, typename... Args>\nauto operator()(F && f, Args &&... args) const"
---

The `stlab::future` returned by this routine must be retained, or the task it is associated with will be cancelled if it has not already been executed.
