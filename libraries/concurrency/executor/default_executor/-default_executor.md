---
layout: method
title: stlab::default_executor
tags: [library]
pure-name: default_executor
defined-in-header: stlab/default_executor.hpp 
declaration: ~default_executor()
description: Destroys the default_executor object. This does not mean that the underlying thread pool gets destroyed. All functions that were submitted, will be executed regardless if this instance will be destroyed.
entities:
  - kind: methods
    list:
      - name: stlab::default_executor::~default_executor
        pure-name: default_executor
        defined-in-header: stlab/default_executor.hpp 
        declaration: ~default_executor()
        description: Destructs a default_executor object
  - kind: example
    code: NoCode
---
