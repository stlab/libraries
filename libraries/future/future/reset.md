---
layout: method
title: stlab::future
tags: [library]
full-name: stlab::future::reset
pure-name: reset
defined-in-header: stlab/future.hpp 
declaration: reset()
description: Cancel this future
entities:
  - kind: methods
    list:
      - name: stlab::future::reset
        pure-name: reset
        defined-in-header: stlab/future.hpp 
        declaration: void reset()
        description: Cancels this future. If the task is already running, its result will be abandoned and no subsequent continuation will be started.
  - kind: example
    code: NoCode
---