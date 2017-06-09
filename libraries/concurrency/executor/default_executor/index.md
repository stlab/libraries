---
layout: class
title: stlab::default_executor
tags: [library]
name: stlab::default_executor
pure-name: default_executor
defined-in-header: executor/concurrency/default_executor.hpp
declaration: struct default_executor
brief: Interface to the system's thread pool
description: Executes functions on the system's thread pool. On MacOS, Windows, Emscripten and PNaCl the OS integrated thread pool is the base of this class. For other OS an custom thread pool is provided. 
member-types:
  - type: result_type
    definition: void
---
