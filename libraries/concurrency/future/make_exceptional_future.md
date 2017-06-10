---
layout: free-function
title: stlab::make_exceptional_future
tags: [library]
pure-name: make_exceptional_future
brief: Creates a future that is fulfilled as failed
description: Creates a future that is fulfilled as failed
annotation: template function
defined-in-header: stlab/concurrency/future.hpp
git-link: https://github.com/stlab/libraries/blob/develop/stlab/concurrency/future.hpp
entities:
  - kind: overloads
    name: stlab::make_exceptional_future
    list:
      - name: make_exceptional_future
        pure-name: make_exceptional_future
        declaration: |
            template <typename T>
            future<T> make_exceptional_future(std::exception_ptr error)
        description: This function creates a future that is fulfilled as failed.
  - kind: parameters
    list:
      - name: error
        description: The exception pointer to the exception that shall be the result of the fulfilled future
  - kind: result
    description: a future that is already in an error state.
---
