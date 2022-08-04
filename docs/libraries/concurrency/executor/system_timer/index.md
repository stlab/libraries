---
layout: class
title: stlab::system_timer
tags: [library]
name: stlab::system_timer
pure-name: system_timer
defined-in-header: stlab/concurrency/system_timer.hpp
declaration: struct system_timer
brief: Executes a function after a specified duration.
ctor: default
dtor: default
description: |
    All functions that are passed to an instance of this class are executed after a specified duration.

    [deprecated] All functions that are passed to an instance of this class are executed at the specified time.

    Upon destruction of the system_timer, the underlying main loop is _not_ destroyed. Likewise, all functions submitted to the executor will be executed regardless of destruction state.
member-types:
  - type: result_type
    definition: void
---
