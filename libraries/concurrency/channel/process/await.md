---
layout: method
title: await
tags: [library]
pure-name: await
declaration: void await(T... val)
brief: Called on an await-process whenever a new value was received from upstream.
description: This method is called on an await-process whenever a new value was received from upstream. The type `T` stands here for any semi regular or move-only type. The number of arguments depends on the number of attached upstream sender. Potential state changes from awaitable to yieldable should happen while this method is invoked.
example: await_example.cpp
entities:
  - kind: methods
    list:
      - name: process::await
        pure-name: await
        declaration: void await(T... val)
        description: This method is called with the next set of values
  - kind: parameters
    list:
      - name: val
        description: The value(s) that are passed to `await`
  - kind: result
---
