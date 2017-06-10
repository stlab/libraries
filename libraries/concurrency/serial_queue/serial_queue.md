---
layout: method
title: stlab::serial_queue::serial_queue
tags: [library]
scope: serial_queue
pure-name: serial_queue
defined-in-header: stlab/concurrency/serial_queue.hpp
git-link: https://github.com/stlab/libraries/blob/develop/stlab/concurrency/serial_queue.hpp
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
        description: Moves the passed function `f` into the serial_queue
  - kind: parameters
    list:
      - name: e
        description: Executor to be invoked when the serial queue executes tasks.
      - name: mode
        description: |
            Specify the mode by which the serial queue processes tasks:

            - `schedule_mode::single` &mdash; Dequeue the tasks one at a time, yielding
              to the executor after every invocation.
            - `schedule_mode::all` &mdash; Will execute all tasks available at the start
              of execution, finally yielding to the executor after they have all run.

---
