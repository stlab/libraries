---
layout: class
title: main_executor_type
owner: sean-parent
brief: Executes tasks on the application's main loop.
tags:
  - class
defined_in_file: concurrency/main_executor.hpp
declaration: "\nstruct stlab::detail::main_executor_type;"
ctor: unspecified
dtor: unspecified
typedefs:
  result_type:
    definition: void
    description: __MISSING__
namespace:
  - stlab
  - v1
  - detail
---

All tasks that are passed to an instance of this instance are executed in-order on the application's main loop.

Upon destruction of the main_executor, the underlying main loop is _not_ destroyed. Likewise, all functions submitted to the executor will be executed regardless of destruction state.

So far no main_executor is implemented for Windows. If `QT_CORE_LIB` is defined and not `STLAB_DISABLE_QT_MAIN_EXECUTOR`, a main executor is instantiated that executes the tasks on the Qt main loop.