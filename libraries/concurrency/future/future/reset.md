---
layout: method
title: reset
tags: [library]
scope: future
pure-name: reset
defined-in-header: stlab/concurrency/future.hpp 
declaration: reset()
description: Cancel this future
entities:
  - kind: methods
    list:
      - name: stlab::future::reset
        pure-name: reset
        defined-in-header: stlab/concurrency/future.hpp 
        declaration: void reset()
        description: Cancels this future. If the task is already running, its result will be abandoned and no subsequent continuation will be started.
  - kind: parameters
  - kind: result
  - kind: example
    code: NoCode
---
