---
layout: method
title: executor
owner: sean-parent
brief: Constructs an executor from the serial queue
tags:
  - method
defined_in_file: concurrency/serial_queue.hpp
overloads:
  __lambda executor() const:
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: __lambda executor() const
---

Like other executors, this routine returns an executor whose `operator()` returns `void`. As such, the tasks enqueued to the serial queue this way are not cancelable, nor is there a future through which their results are returned.

This is a good means of passing "fire and forget" tasks into the serial queue.
