---
layout: function
title: valid
brief: Returns `true` if the future is valid.
defined-in-header: stlab/concurrency/future.hpp 
tags:
  - function
overloads:
  bool valid():
    description: Returns `true` if the future is connected with a valid task. A default constructed object or a canceled one returns `false`.
    return: returns `true` if the future has an associated function object, otherwise `false`; after a reset, it returns `false`
    signature_with_names: bool valid() const
---
