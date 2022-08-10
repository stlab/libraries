---
layout: library
title: concurrency/default_executor.hpp
owner: sean-parent
brief: Interface to the system's thread pool
tags:
  - sourcefile
library-type: sourcefile
---

The `stlab.concurrency` library provides an abstraction to the OS's system thread pool. It provides its own implementation if the OS does not provide one. 

Currently the abstraction is available on MacOS, Windows, Emscripten and PNaCl. 

Tasks can be submitted with different priority by using the `high_executor`, `default_executor` or `low_executor`. Tasks submitted to the executors are executed in the order of high, default and then low. It is not guaranteed that this order is strictly kept, because of performance reasons.