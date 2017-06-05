---
layout: method
title: stlab::default_executor
tags: [library]
pure-name: default_executor
defined-in-header: stlab/default_executor.hpp 
declaration: default_executor()
description: Constructs a new default_executor object. All functions that are passed to an instance of this class are executed on the system's thread pool.
entities:
  - kind: methods
    list:
      - name: stlab::default_executor::default_executor
        pure-name: default_executor
        defined-in-header: stlab/default_executor.hpp 
        declaration: default_executor()
        description: Constructs a new default_executor object. Normaly all instances of this class refer to the same systems thread pool. In case that the operting system does not provide a thread pool, these instances refer to the same custom thread pool.
  - kind: parameters
  - kind: example
    code: NoCode
---
