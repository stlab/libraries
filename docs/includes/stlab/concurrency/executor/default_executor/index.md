---
layout: class
title: stlab::default_executor
tags:
  - class
defined-in-header: stlab/concurrency/default_executor.hpp
brief: Interface to the system's thread pool
description: "The stlab.concurrency library provides an abstraction to the OS's system thread pool. It provides its own implementation if the OS does not provide one. \nCurrently the abstraction is available on MacOS, Windows, Emscripten and PNaCl. \nTasks can be submitted with different priority by using the `high_executor`, `default_executor` or `low_executor`. Tasks submitted to the executors are executed in the order of high, default and then low. It is not guaranteed that this order is strictly kept, because of performance reasons."
member-types:
  - type: result_type
    definition: void

namespace:
  - stlab
---
