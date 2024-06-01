---
layout: method
title: stlab::executor
tags: [library]
scope: stlab
pure-name: executor
defined-in-header: stlab/concurrency/channel.hpp 
declaration: executor()
brief: Constructs a new executor wrapper object
description: Constructs a new executor wrapper object
entities:
  - kind: methods
    list:
      - name: stlab::executor::executor
        pure-name: executor
        defined-in-header: stlab/concurrency/channel.hpp 
        declaration: executor(executor_t e)
        description: Constructs a new executor object and stores the given executor in it.
  - kind: parameters
    list:
      - name: e
        description: Specifies the executor
---
