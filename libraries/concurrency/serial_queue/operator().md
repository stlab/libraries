---
layout: method
title: stlab::serial_queue::operator()
tags: [library]
scope: serial_queue
pure-name: operator()
defined-in-header: stlab/concurrency/serial_queue.hpp
brief: Passes the task to the serial_queue for scheduling and execution
entities:
  - kind: methods
    list:
      - name: stlab::serial_queue::operator()
        pure-name: operator()
        defined-in-header: stlab/concurrency/serial_queue.hpp 
        declaration: |
            template <typename F, typename... Args>
            auto operator()(F&& f, Args&&... args)
        description: Pass a task to the serial_queue for execution
  - kind: result
    description: Returns a future that will contain the result of the executed task.
---

The `stlab::future` returned by this routine must be retained, or the task it is associated with will be cancelled if it has not already been executed.
