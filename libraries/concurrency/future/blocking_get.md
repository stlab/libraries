---
layout: function
title: stlab::blocking_get
brief: Waits until the future is ready
defined-in-header: stlab/concurrency/utility.hpp
tags:
  - function
overloads:
  "template<typename T>\nT blocking_get(future<T>)":
    description: Waits as long as the future is fulfilled, or throws the occurred exception.
    arguments:
      - description: The future on which shall be waited until the result is ready.
        name: x
        type: future<T>
    return: The result of the future x
    signature_with_names: template<T>\nT blocking_get(future<T> x)
  "void blocking_get(future<void>)":
    description: Waits as long as the future is fulfilled, or throws the occurred exception.
    arguments:
      - description: The future on which shall be waited until the result is ready.
        name: x
        type: future<void>
    signature_with_names: void blocking_get(future<void> x)
---
Waits until the passed future is ready. Using this function causes that one thread resource is consumed which increases contention and possibly causing a deadlock. As well any subsequent non-dependent calculations on the task are also blocked. It is strongly recommended to avoid using this function, but use continuations.