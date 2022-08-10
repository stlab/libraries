---
layout: function
title: operator!=
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: concurrency/future.hpp
overloads:
  bool operator!=(const future<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &, const future<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: const future<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &
      - description: __OPTIONAL__
        name: y
        type: const future<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: bool operator!=(const future<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> & x, const future<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> & y)
  bool operator!=(const future<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> &, const future<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> &):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: const future<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> &
      - description: __OPTIONAL__
        name: y
        type: const future<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: bool operator!=(const future<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> & x, const future<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> & y)
  bool operator!=(const stlab::future<stlab::future<void>> &, const stlab::future<stlab::future<void>> &):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: const stlab::future<stlab::future<void>> &
      - description: __OPTIONAL__
        name: y
        type: const stlab::future<stlab::future<void>> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: bool operator!=(const stlab::future<stlab::future<void>> & x, const stlab::future<stlab::future<void>> & y)
  bool operator!=(const stlab::future<void> &, const stlab::future<void> &):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: const stlab::future<void> &
      - description: __OPTIONAL__
        name: y
        type: const stlab::future<void> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: bool operator!=(const stlab::future<void> & x, const stlab::future<void> & y)
namespace:
  - stlab
  - v1
---
