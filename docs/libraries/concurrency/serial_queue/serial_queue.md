---
layout: method
title: stlab::serial_queue::serial_queue
tags: [library]
scope: serial_queue
pure-name: serial_queue
defined-in-header: stlab/concurrency/serial_queue.hpp
brief: Constructs a serial_queue
entities:
  - kind: methods
    list:
      - name: stlab::serial_queue::serial_queue
        pure-name: serial_queue
        defined-in-header: stlab/concurrency/serial_queue.hpp 
        declaration: |
            template <typename Executor>
            explicit serial_queue_t(Executor e, schedule_mode mode = schedule_mode::single)
        description: Constructs the serial queue backed by the passed executor and using the supplied schedule mode.
  - kind: parameters
    list:
      - name: e
        description: The executor the serial queue will use to run tasks
      - name: mode
        description: |
            Specify the mode by which the serial queue runs tasks:

            - `schedule_mode::single` &mdash; Yield to the executor between runs of individual tasks. This mode is preferrable if you are expecting tasks to take a long amount of time.
            - `schedule_mode::all` &mdash; Yield to the executor once all tasks available at the start of processing have been run. This mode is preferrable if you are expecting tasks to take a short amount of time.
---
