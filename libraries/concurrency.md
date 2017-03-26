---
layout: library
title: Concurrency
tags: [library]
description: This library provides high level constructs for implementing algorithms that eases the use of multiple CPU cores while minimizing the contention.
sections:
  - kind: collection
    name: Futures
    description: An alternative future implementation, that supports continuations, splits and joins.
    entities:
      - kind: class
        name: stlab::future
        pure-name: future
        defined-in-header: stlab/future.hpp
        declaration: future
        description: |
            This class implements a future
            <span class="t-mark">(class template)</span>
        link: future/stlab_future
  - kind: collection
    name: Channels
    description: CSP channels that allow to greate processing graphs that execute the tasks on dedicated executors. It is possible to split, join, zip and merge channels. Each processing node is associated with a [process](channel/policy_process). 
    entities:
      - kind: class
        name: stlab::sender
        pure-name: sender
        defined-in-header: stlab/channel.hpp
        declaration: sender
        description: |
            This class implements the sending part of a CSP channel

            <span class="t-mark">(class template)</span>
        link: channel/stlab_sender
      - kind: class
        name: stlab::receiver
        pure-name: receiver
        defined-in-header: stlab/channel.hpp
        declaration: receiver
        description: |
            This class implements the receiving part of a CSP channel

            <span class="t-mark">(class template)</span>
        link: channel/stlab_receiver
      - kind: class
        name: process
        pure-name: process
        declaration: process
        description: |
            Describes a process that gets executed whenever a value is passed into the channel

            <span class="t-mark">(template policy)</span>
        link: channel/policy_process
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

### Requirements ###

* C++14 compliant compiler (clang 3.6, gcc 5.3, Visual Studio 2015 Update 3)
* boost 1.60.0 (optional, variant and test for executing the UnitTests)

### Authors ###
Sean Parent, Felix Petriconi
