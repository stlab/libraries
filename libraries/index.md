---
layout: library
title: libraries
description: This library provides high level constructs for implementing algorithms that eases the use of multiple CPU cores while minimizing the contention.
authors: Sean Parent, Felix Petriconi
sections:
  - kind: collection
    name: Futures
    description: Alternative future implementation
    entities:
      - kind: class
        name: stlab::future
        pure-name: future
        defined-in-header: stlab/future.hpp
        declaration: future
        description: This class implements a future
        link: future/stlab_future
  - kind: collection
    name: Channels
    description: CSP channels
    entities:
      - kind: class
        name: stlab::sender
        pure-name: sender
        defined-in-header: stlab/channel.hpp
        declaration: sender
        description: This class implements the sending part of a CSP channel
        link: channel/stlab_sender
      - kind: class
        name: stlab::receiver
        pure-name: receiver
        defined-in-header: stlab/channel.hpp
        declaration: receiver
        description: This class implements the receiving part of a CSP channel
        link: channel/stlab_receiver
  - kind: collection
    name: Executors
    description: Collection of executors
    entities:
      - kind: class
        name: stlab::default_executor
        pure-name: default_executor
        defined-in-header: stlab/default_executor.hpp
        declaration: default_executor
        description: This class implements an executor that schedules the tasks on a thread pool
        link: executor/stlab_default_executor
      - kind: class
        name: stlab::main_executor
        pure-name: main_executor
        defined-in-header: stlab/main_executor.hpp
        declaration: main_executor
        description: This class implements an executor that schedules the tasks on the main loop
        link: executor/stlab_main_executor
      - kind: class
        name: stlab::immediate_executor
        pure-name: immediate_executor
        defined-in-header: stlab/immediate_executor.hpp
        declaration: immediate_executor
        description: This class implements an executor that executes the task immediately
        link: executor/stlab_immediate_executor
---
