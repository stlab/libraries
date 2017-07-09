---
layout: method
title: stlab::serial_queue::executor
tags: [library]
scope: serial_queue
pure-name: executor
defined-in-header: stlab/concurrency/serial_queue.hpp
brief: Constructs an executor from the serial queue
entities:
  - kind: methods
    list:
      - name: stlab::serial_queue::executor
        pure-name: executor
        defined-in-header: stlab/concurrency/serial_queue.hpp 
        declaration: |
            auto executor() const
        description: Projects the serial queue into an executor.
  - kind: result
    description: An executor that serializes execution of tasks.
---

Like other executors, this routine returns an executor whose `operator()` returns `void`. As such, the tasks enqueued to the serial queue this way are not cancelable, nor is there a future through which their results are returned.

This is a good means of passing "fire and forget" tasks into the serial queue.
