---
layout: free-function
title: stlab::make_ready_future
tags: [library]
pure-name: make_ready_future
brief: Creates a future that is already fulfilled
description: Creates a future that is already fulfilled
entities:
  - kind: overloads
    name: stlab::make_ready_future
    defined-in-header: stlab/future.hpp
    git-link: https://github.com/stlab/libraries/blob/develop/stlab/future.hpp
    list:
      - name: make_ready_future
        pure-name: make_ready_future
        declaration: |
            template <typename T>
            future<T> make_ready_future(T&& x)
        description: This function creates a future that is already fulfilled.
      - name: make_ready_future
        pure-name: make_ready_future
        declaration: |
            future<void> make_ready_future()
        description: This function creates a future of type `void` that is already fulfilled.
  - kind: parameters
    list:
      - name: x
        description: The result value of the future
  - kind: result
    description: a future that is already fulfilled.
---
