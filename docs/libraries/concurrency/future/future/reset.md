---
layout: function
title: reset
brief: Cancels this future
defined-in-header: stlab/concurrency/future.hpp 
tags:
  - function
overloads:
  void reset():
    description: Cancels this future. If the task is already running, its result will be abandoned and no subsequent continuation will be started.
    return: __OPTIONAL__
    signature_with_names: void reset()
---
