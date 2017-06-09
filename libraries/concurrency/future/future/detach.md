---
layout: method
title: detach
tags: [library]
scope: future
pure-name: detach
defined-in-header: stlab/concurrency/future.hpp 
declaration: detach()
description: Detaches the associated tasks from this instance. So it will be executed even this instance get destructed.
entities:
  - kind: methods
    list:
      - name: stlab::future::detach
        pure-name: detach
        defined-in-header: stlab/concurrency/future.hpp 
        declaration: void detach() const
        description: Lets the current task run and detaches it from this instance.
  - kind: parameters
  - kind: result
  - kind: example
    code: NoCode
---
