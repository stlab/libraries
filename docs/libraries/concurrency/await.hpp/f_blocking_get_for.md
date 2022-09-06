---
layout: function
title: blocking_get_for
owner: sean-parent
brief: _Deprecated_ Use [`await_for()`](./f_await_for.html).
tags:
  - function
defined_in_file: concurrency/await.hpp
overloads:
  "template <class T>\nauto blocking_get_for(future<T>, const std::chrono::nanoseconds &) -> future<T>":
    annotation:
      - deprecated (Use await_for instead.)
    arguments:
      - description: __OPTIONAL__
        name: x
        type: future<T>
      - description: __OPTIONAL__
        name: timeout
        type: const std::chrono::nanoseconds &
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <class T>\nauto blocking_get_for(future<T> x, const std::chrono::nanoseconds & timeout) -> future<T>"
namespace:
  - stlab
  - v1
---
