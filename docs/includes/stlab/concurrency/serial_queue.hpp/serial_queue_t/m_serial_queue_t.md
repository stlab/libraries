---
layout: method
title: serial_queue_t
hyde:
  owner: sean-parent
  brief: Constructs a serial queue
  tags:
    - method
  defined_in_file: stlab/concurrency/serial_queue.hpp
  is_ctor: true
  overloads:
    serial_queue_t(const stlab::serial_queue_t &):
      annotation:
        - implicit
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: const stlab::serial_queue_t &
          unnamed: true
      description: Constructs the serial queue backed by the passed executor and using the supplied schedule mode.
      signature_with_names: serial_queue_t(const stlab::serial_queue_t &)
    serial_queue_t(stlab::serial_queue_t &&):
      annotation:
        - implicit
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: stlab::serial_queue_t &&
          unnamed: true
      description: Constructs a serial_queue from another serial_queue.
      signature_with_names: serial_queue_t(stlab::serial_queue_t &&)
    "template <typename Executor>\nexplicit serial_queue_t(Executor, stlab::schedule_mode)":
      arguments:
        - description: The executor the serial queue will use to run tasks
          name: e
          type: Executor
        - description: "Specify the mode by which the serial queue runs tasks:\n\n- `schedule_mode::single` &mdash; Yield to the executor between runs of individual tasks. This mode is preferrable if you are expecting tasks to take a long amount of time.\n- `schedule_mode::all` &mdash; Yield to the executor once all tasks available at the start of processing have been run. This mode is preferrable if you are expecting tasks to take a short amount of time.\n"
          name: mode
          type: stlab::schedule_mode
      description: __MISSING__
      signature_with_names: "template <typename Executor>\nexplicit serial_queue_t(Executor e, stlab::schedule_mode mode)"
---
