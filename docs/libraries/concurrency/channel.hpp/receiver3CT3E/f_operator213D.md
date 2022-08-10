---
layout: function
title: operator!=
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: concurrency/channel.hpp
overloads:
  bool operator!=(const receiver<T> &, const receiver<T> &):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: const receiver<T> &
      - description: __OPTIONAL__
        name: y
        type: const receiver<T> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: bool operator!=(const receiver<T> & x, const receiver<T> & y)
  bool operator!=(const sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &, const sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: const sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &
      - description: __OPTIONAL__
        name: y
        type: const sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: bool operator!=(const sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> & x, const sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> & y)
  bool operator!=(const sender<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> &, const sender<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> &):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: const sender<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> &
      - description: __OPTIONAL__
        name: y
        type: const sender<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: bool operator!=(const sender<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> & x, const sender<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> & y)
namespace:
  - stlab
  - v1
---
