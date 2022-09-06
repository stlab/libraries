---
layout: function
title: await
owner: sean-parent
brief: Waits until the future is ready and returns the future value.
tags:
  - function
defined_in_file: concurrency/await.hpp
overloads:
  "template <typename T>\nT await(future<T>)":
    arguments:
      - description: __OPTIONAL__
        name: x
        type: future<T>
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <typename T>\nT await(future<T> x)"
namespace:
  - stlab
  - v1
---
