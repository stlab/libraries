---
layout: method
title: stlab::main_executor
tags: [library]
full-name: stlab::main_executor::~main_executor
pure-name: main_executor
defined-in-header: stlab/main_executor.hpp 
declaration: ~main_executor()
description: Destroys a main_executor object. This does not mean that the underlying main loop gets destroyed. All functions that were submitted, will be executed regardless if this instance will be destroyed.
entities:
  - kind: methods
    list:
      - name: stlab::main_executor::~main_executor
        pure-name: main_executor
        defined-in-header: stlab/main_executor.hpp 
        declaration: ~main_executor()
        description: Destructs a main_executor object
  - kind: example
    code: NoCode
---
