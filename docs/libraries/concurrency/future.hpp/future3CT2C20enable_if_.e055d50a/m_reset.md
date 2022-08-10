---
layout: method
title: reset
owner: sean-parent
brief: Cancels this future
tags:
  - method
defined_in_file: concurrency/future.hpp
overloads:
  void reset():
    description: Cancels this future. If the task is already running, its result will be abandoned and no subsequent continuation will be started.
    return: __OPTIONAL__
    signature_with_names: void reset()
---
