---
layout: function
title: stlab::blocking_get
tags: [library]
pure-name: blocking_get
brief: Waits until the future is ready
description: |
    Waits until the passed future is ready. Using this function causes that one thread resource is consumed which increases contention and possibly causing a deadlock. As well any subsequent non-dependent calculations on the task are also blocked. It is strongly recommended to avoid using this function, but use continuations.
annotation: template function
defined-in-header: stlab/concurrency/utility.hpp
entities:
  - kind: overloads
    name: stlab::blocking_get
    list:
      - name: blocking_get
        pure-name: blocking_get
        declaration: |
            void blocking_get(future<void> x)
        description: Waits as long as the future is fulfilled, or throws the occurred exception.
      - name: blocking_get
        pure-name: blocking_get
        declaration: |
            template <typename T>
            T blocking_get(future<T> x)
        description: Returns the value of the fulfilled future, or throws the occurred exception.
  - kind: parameters
    list:
      - name: x
        description: The future on which shall be waited.
  - kind: result
    description: The stored value or void.
---
