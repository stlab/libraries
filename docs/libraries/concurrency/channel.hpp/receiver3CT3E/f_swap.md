---
layout: function
title: swap
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: concurrency/channel.hpp
overloads:
  void swap(receiver<T> &, receiver<T> &):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: receiver<T> &
      - description: __OPTIONAL__
        name: y
        type: receiver<T> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: void swap(receiver<T> & x, receiver<T> & y)
  void swap(sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &, sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &
      - description: __OPTIONAL__
        name: y
        type: sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: void swap(sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> & x, sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> & y)
  void swap(sender<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> &, sender<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> &):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: sender<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> &
      - description: __OPTIONAL__
        name: y
        type: sender<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: void swap(sender<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> & x, sender<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> & y)
namespace:
  - stlab
  - v1
---
