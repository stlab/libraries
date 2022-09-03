---
layout: library
title: pre_exit.hpp
owner: sean-parent
brief: Utilities to handle the invocation of operations that must happen prior to program exit.
tags:
  - sourcefile
library-type: sourcefile
typedefs:
  pre_exit_handler:
    definition: void (*)() noexcept
    description: A function to be invoked prior on `pre_exit()`.
---

Utilities to register and invoke operations that must be executed prior to exit. `pre_exit()` must be invoked explicitely before program exit (via `std::exit()` or exiting `main()`).
