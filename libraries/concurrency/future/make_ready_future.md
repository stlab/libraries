---
layout: free-function
title: stlab::make_ready_future
tags: [library]
pure-name: make_ready_future
entities:
  - kind: overloads
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
      - name: make_exceptional_future
        pure-name: make_exceptional_future
        declaration: |
            template <typename T>
            future<T> make_exceptional_future(std::exception_ptr error)
        description: This function creates a future that is fulfilled as failed.
  - kind: parameters
    list:
      - name: x
        description: The result value of the future
      - name: error
        description: The exception pointer to the exception that shall be the result of the fulfilled future
  - kind: result
    description: a future that already is fulfilled.
---
