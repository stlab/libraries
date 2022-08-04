---
layout: function
title: detach
brief: Detaches the associated tasks from this instance.
defined-in-header: stlab/concurrency/future.hpp 
tags:
  - function
overloads:
  void detach():
    description: Lets the current task run and detaches it from this instance.
    return: __OPTIONAL__
    signature_with_names: void detach()
---
Detaches the associated tasks from this instance. So it will be executed even when this instance get destructed.

