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

Currently the abstraction is available on Apple platforms, Windows, and WASM.

Tasks can be submitted with different priority by using the `high_executor`, `default_executor` or `low_executor`. Tasks submitted to the executors are executed in the order of high, default and then low. It is not guaranteed that this order is strictly kept, because of performance reasons.

_NOTE: [`pre_exit()`](../../pre_exit.hpp/f_pre_exit.html) must be called prior to program exit if the default executor is used. The default executor uses a [`pre_exit_handler`](../../pre_exit.hpp/) to ensure that any detached tasks (such as a canceled task) do not run concurrently with exit handlers and the destruction of global variables. Alternatively, invoking [`std::quick_exit()`](https://en.cppreference.com/w/cpp/utility/program/quick_exit), or an equivalent, can be used if appropriate for the program._
