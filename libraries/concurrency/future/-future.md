---
layout: method
title: stlab::future
tags: [library]
full-name: stlab::future::~future
pure-name: ~future
defined-in-header: stlab/future.hpp
declaration: ~future()
brief: Destructs the future
description: Destructs the future object. A not started task will be canceled. An already running task will be abandoned and no potentially attached continuation will be triggered.
entities:
  - kind: methods
    list:
      - name: stlab::future::~future
        pure-name: ~future
        defined-in-header: stlab/future.hpp 
        declaration: ~future()
        description: Destructs a future object
  - kind: example
    code: NoCode
---
