---
layout: library
title: Concurrency
tags: [library]
description: |
    This library provides high level constructs for implementing algorithms that eases the use of multiple CPU cores while minimizing the contention.
    The future implementaton differs in several aspects compared to the C++11/14/17 standard futures: It provides continuations and joins, which were just added in a C++17 TS. But more important this futures propagate values through the graph and not futures. This allows an easy way of creating splits. That means a single future can have multiple continuations into different directions. An other important difference is that the futures support cancellation. So if one is not anymore interested in the result of a future, then one can destroy the future without the need to wait that the future is fullfilled, as it is the case with std::future (and boost::future). An already started future will run until its end, but will not trigger any continuation. So in all these cases, all chained continuations will never be triggered. Additionally the future interface is designed in a way, that one can use build in or custom build executors. 
    Since one can create with futures only graphs for single use, this library provides as well channels. With these channels one can build graphs, that can be used for multiple invocations.
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
        annotation: template
        link: future/index
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
        link: channel/sender
        annotation: template
      - kind: class
        name: stlab::receiver
        pure-name: receiver
        defined-in-header: stlab/channel.hpp
        declaration: receiver
        description: |
            This class implements the receiving part of a CSP channel
        link: channel/receiver
        annotation: template
      - kind: class
        name: process
        pure-name: process
        declaration: process
        description: |
            Describes a process that gets executed whenever a value is passed into the channel
        link: channel/process
        annotation: concept
  - kind: collection
    name: Executors
    description: Collection of executors
    entities:
      - kind: class
        name: stlab::executor
        pure-name: executor
        defined-in-header: stlab/executor.hpp
        declaration: executor
        description: An executor to be piped to a `receiver<T>`
        link: executor/executor
      - kind: class
        name: stlab::default_executor
        pure-name: default_executor
        defined-in-header: stlab/default_executor.hpp
        declaration: default_executor
        description: An executor that schedules tasks on a thread pool
        link: executor/default_executor
      - kind: class
        name: stlab::main_executor
        pure-name: main_executor
        defined-in-header: stlab/main_executor.hpp
        declaration: main_executor
        description: An executor that schedules tasks on the main loop
        link: executor/main_executor
      - kind: class
        name: stlab::immediate_executor
        pure-name: immediate_executor
        defined-in-header: stlab/immediate_executor.hpp
        declaration: immediate_executor
        description: An executor that executes tasks immediately
        link: executor/immediate_executor
---

### Requirements ###

* C++14 compliant compiler (clang 3.6, gcc 5.3, Visual Studio 2015 Update 3)
* boost 1.60.0 (optional, variant and test for executing the UnitTests)

### Authors ###
Sean Parent, Felix Petriconi
