---
layout: method
title: get_try
owner: sean-parent
brief: Returns the result of the future.
tags:
  - method
defined_in_file: concurrency/future.hpp
overloads:
  auto get_try() &&:
    description: If `T` is not `void`, returns an initialized `optional` if the future has succeeded, otherwise an empty `optional<T>`. Rethrows any caught exceptions if an error occurred.
    return: __OPTIONAL__
    signature_with_names: auto get_try() &&
  auto get_try() const &:
    description: If `T` is void, returns `true` if the future succeeded, otherwise `false`. Rethrows any caught exceptions if an error occurred.
    return: __OPTIONAL__
    signature_with_names: auto get_try() const &
---
If `T` is of type `void`, it returns `true` if the future is fulfilled; if `T` is a non-`void` type then it returns an `optional<T>` with the result of the future if the future is already fulfilled, otherwise an empty `optional<T>`
