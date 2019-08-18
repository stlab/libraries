---
layout: function
title: await
brief: Called on an await-process whenever a new value was received from upstream.
tags:
  - function
example: await_example.cpp
overloads:
  void await(T... val):
    description: This method is called with the next set of values
    arguments:
      - description: The value(s) that are passed to `await`
        name: val
        type: T
    return: __OPTIONAL__
    signature_with_names: void await(T... val)
---
This method is called on an await-process whenever a new value was received from upstream. The type `T` stands here for any semi regular or move-only type. The number of arguments depends on the number of attached upstream sender. Potential state changes from awaitable to yieldable should happen while this method is invoked.
