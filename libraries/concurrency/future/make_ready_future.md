---
layout: function
title: stlab::make_ready_future
tags: [library]
pure-name: make_ready_future
brief: Creates a future that is already fulfilled
description: Creates a future that is already fulfilled
annotation: template function
defined-in-header: stlab/concurrency/future.hpp
entities:
  - kind: overloads
    name: stlab::make_ready_future
    list:
      - name: make_ready_future
        pure-name: make_ready_future
        declaration: |
            template <typename T, typename E>
            future<T> make_ready_future(T&& x, E executor)
        description: This function creates a future that is already fulfilled.
      - name: make_ready_future
        pure-name: make_ready_future
        declaration: |
            template <typename E>
            future<void> make_ready_future(E executor)
        description: This function creates a future of type `void` that is already fulfilled.
  - kind: parameters
    list:
      - name: x
        description: The result value of the future
      - name: executor
        description: The executor that shall be used to create this ready future        
  - kind: result
    description: a future that is already fulfilled.
---
