---
layout: library
title: libraries
entities:
  - kind: class
    name: stlab::future
    pure-name: future
    defined-in-header: stlab/future.hpp
    declaration: |
        template <typename T> class future;
    description: This class implements a future ibrary
    link: future/stlab_future
  - kind: class
    name: stlab::sender
    pure-name: sender
    defined-in-header: stlab/channel.hpp
    declaration: |
        template <typename T> class sender;
    description: This class implements the sending part of a CSP channel
    link: channel/stlab_sender
  - kind: class
    name: stlab::receiver
    pure-name: receiver
    defined-in-header: stlab/channel.hpp
    declaration: |
        template <typename T> class receiver;
    description: This class implements the receiving part of a CSP channel
    link: channel/stlab_receiver
  - kind: class
    name: stlab::default_executor
    pure-name: default_executor
    defined-in-header: stlab/default_executor.hpp
    declaration: struct default_executor;
    description: This class implements an executor that schedules the tasks on a thread pool
    link: executor/default_executor
  - kind: class
    name: stlab::main_executor
    pure-name: main_executor
    defined-in-header: stlab/main_executor.hpp
    declaration: struct main_executor
    description: This class implements an executor that schedules the tasks on the main loop
    link: executor/main_executor
  - kind: class
    name: stlab::immediate_executor
    pure-name: immediate_executor
    defined-in-header: stlab/immediate_executor.hpp
    declaration: immediate_executor
    description: This class implements an executor that executes the task immediately
    link: executor/immediate_executor
---
