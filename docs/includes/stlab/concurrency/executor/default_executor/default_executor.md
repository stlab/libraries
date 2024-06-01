---
layout: function
title: stlab::default_executor
brief: Provides access to a thread pool
defined-in-header: stlab/concurrency/default_executor.hpp 
tags:
  - function
overloads:
  stlab::default_executor(task<void()>):
    description: Provides access the executor with default priority.
    arguments:
      - description: The task that shall be executed by this executor
        name: f
        type: stlab::task<void()>
    return: __OPTIONAL__
    signature_with_names: default_executor(task<void()> f)
  stlab::high_executor(task<void()>):
    description: Provides access the executor with high priority.
    arguments:
      - description: The task that shall be executed by this executor
        name: f
        type: stlab::task<void()>
    return: __OPTIONAL__
    signature_with_names: high_executor(task<void()> f)
  stlab::low_executor(task<void()>):
    description: Provides access the executor with low priority.
    arguments:
      - description: The task that shall be executed by this executor
        name: f
        type: stlab::task<void()>
    return: __OPTIONAL__
    signature_with_names: low_executor(task<void()> f)
---
Provides access to the default executor for this process. Normally all instances of this class refer to the same systems thread pool. In case that the operating system does not provide a thread pool, these instances refer to the same custom thread pool. Task submitted to the different instances are executed in the order of the priority high, default and low. Because of performance reasons it cannot be guaranteed that this order is kept strictly.