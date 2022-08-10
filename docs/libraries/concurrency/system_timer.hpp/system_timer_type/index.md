---
layout: class
title: system_timer_type
owner: sean-parent
brief: Executes a function after a specified duration.
tags:
  - class
defined_in_file: concurrency/system_timer.hpp
declaration: "\nstruct stlab::detail::system_timer_type;"
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

All functions that are passed to an instance of this class are executed after a specified duration.

`[deprecated]` All functions that are passed to an instance of this class are executed at the specified time.

Upon destruction of `system_timer`, the underlying main loop is _not_ destroyed. Likewise, all functions submitted to the executor will be executed regardless of destruction state.