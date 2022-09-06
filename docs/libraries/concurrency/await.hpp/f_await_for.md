---
layout: function
title: await_for
owner: sean-parent
brief: |
  Wait for the specified duration for the future to be ready and either returns a ready future or a new future which can be waited on.
tags:
  - function
defined_in_file: concurrency/await.hpp
overloads:
  "template <class T>\nauto await_for(future<T>, const std::chrono::nanoseconds &) -> future<T>":
    arguments:
      - description: __OPTIONAL__
        name: x
        type: future<T>
      - description: __OPTIONAL__
        name: timeout
        type: const std::chrono::nanoseconds &
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <class T>\nauto await_for(future<T> x, const std::chrono::nanoseconds & timeout) -> future<T>"
namespace:
  - stlab
  - v1
---
