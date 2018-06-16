---
layout: method
title: get_try
tags: [library]
scope: future
pure-name: get_try
defined-in-header: stlab/concurrency/future.hpp 
declaration: get_try()
description: Returns the result of the future.
entities:
  - kind: methods
    list:
      - name: stlab::future::get_try
        pure-name: get_try
        defined-in-header: stlab/concurrency/future.hpp 
        declaration: stlab::optional<T> get_try() const&
        description: If `T` is not `void`, it returns an initialized `optional` if the future has succeeded, otherwise an empty `optional<T>`. In case that an error occurred it rethrows the captured exception.
      - name: stlab::future::then
        pure-name: then
        defined-in-header: stlab/concurrency/future.hpp 
        declaration: bool get_try() const&
        description: If `T` is void it returns `true` if the future succeeded, otherwise `false`. In case that an error occurred it rethrows the captured exception.
  - kind: result
    description: if `T` is of type `void`, it returns `true` if the future is fulfilled; if `T` is a non-`void` type then it returns an `optional<T>` with the result of the future if the future is already fulfilled, otherwise an empty `optional<T>`
---
