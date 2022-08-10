---
layout: function
title: swap
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: concurrency/future.hpp
overloads:
  void swap(future<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &, future<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: future<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &
      - description: __OPTIONAL__
        name: y
        type: future<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: void swap(future<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> & x, future<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> & y)
  void swap(future<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> &, future<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> &):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: future<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> &
      - description: __OPTIONAL__
        name: y
        type: future<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: void swap(future<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> & x, future<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> & y)
  void swap(stlab::future<stlab::future<void>> &, stlab::future<stlab::future<void>> &):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: stlab::future<stlab::future<void>> &
      - description: __OPTIONAL__
        name: y
        type: stlab::future<stlab::future<void>> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: void swap(stlab::future<stlab::future<void>> & x, stlab::future<stlab::future<void>> & y)
  void swap(stlab::future<void> &, stlab::future<void> &):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: stlab::future<void> &
      - description: __OPTIONAL__
        name: y
        type: stlab::future<void> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: void swap(stlab::future<void> & x, stlab::future<void> & y)
namespace:
  - stlab
  - v1
---
