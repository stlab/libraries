---
layout: method
title: yield
tags: [library]
full-name: process::yield
pure-name: yield
declaration: T yield()
brief: Called on an await-process when it should yield
description: This method is called on an await-process whenever the `process_state_scheduled.first` is `process_state::yield` or a timeout was provided with the recent call to `state()` and that has elapsed.
entities:
  - kind: methods
    list:
      - name: process::yield
        pure-name: yield
        declaration: T yield()
        description: This method is called whenever the state has changed from await to yield or a timeout was provided with the recent call to `state()`.
  - kind: parameters
  - kind: result
    description: The result of the process.
---
