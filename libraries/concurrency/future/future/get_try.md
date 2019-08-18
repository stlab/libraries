---
layout: function
title: get_try
brief: Returns the result of the future.
defined-in-header: stlab/concurrency/future.hpp 
tags:
  - function
overloads:
  stlab::optional<T> get_try():
    description: If `T` is not `void`, it returns an initialized `optional` if the future has succeeded, otherwise an empty `optional<T>`. In case that an error occurred it rethrows the captured exception.
    return: __OPTIONAL__
    signature_with_names: stlab::optional<T> get_try()
  bool get_try():
    description: If `T` is void it returns `true` if the future succeeded, otherwise `false`. In case that an error occurred it rethrows the captured exception.
    return: __OPTIONAL__
    signature_with_names: bool get_try()
---
If `T` is of type `void`, it returns `true` if the future is fulfilled; if `T` is a non-`void` type then it returns an `optional<T>` with the result of the future if the future is already fulfilled, otherwise an empty `optional<T>`
