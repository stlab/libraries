---
layout: class
title: stlab::main_executor
tags: [library]
name: stlab::main_executor
pure-name: main_executor
defined-in-header: stlab/concurrency/main_executor.hpp
declaration: struct main_executor
brief: Executes tasks on the application's main loop.
ctor: default
dtor: default
description: |
    All tasks that are passed to an instance of this instance are executed in-order on the application's main loop.

    Upon destruction of the main_executor, the underlying main loop is _not_ destroyed. Likewise, all functions submitted to the executor will be executed regardless of destruction state.
    So far no main_executor is implemented for Windows. If QT_CORE_LIB is defined and not STLAB_DISABLE_QT_MAIN_EXECUTOR, a main executor is instantiated that executes the tasks on the Qt main loop.
member-types:
  - type: result_type
    definition: void
---
