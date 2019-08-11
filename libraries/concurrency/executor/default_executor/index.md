---
layout: class
title: stlab::default_executor
tags: [library]
name: stlab::default_executor
pure-name: default_executor
defined-in-header: stlab/concurrency/default_executor.hpp
declaration: struct default_executor
brief: Interface to the system's thread pool
description: Executes functions on the system's thread pool. On MacOS, Windows, Emscripten and PNaCl the OS integrated thread pool is the base of this class. For other OS an custom thread pool is provided.
A `high_executor` and `low_executor `are available beside the `default_executor`. Tasks submitted to the executors are executed in the order of high, default and then low. It is not guaranteed that this order is strictly kept, because of performance reasons. 
member-types:
  - type: result_type
    definition: void
---
