---
layout: class
title: stlab::main_executor
tags: [library]
name: stlab::main_executor
pure-name: main_executor
defined-in-header: executor/main_executor.hpp
declaration: struct main_executor
brief: Executes functions on the application's main loop.
ctor: default
dtor: default
description: |
    All functions that are passed to an instance of this class are executed in-order on the application's main loop.

    Upon destruction of the main_executor, the underlying main loop is _not_ destroyed. Likewise, all functions submitted to the executor will be executed regardless of destruction state.
member-types:
  - type: result_type
    definition: void
---
