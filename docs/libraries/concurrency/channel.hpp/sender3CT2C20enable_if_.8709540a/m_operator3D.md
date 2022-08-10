---
layout: method
title: operator=
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: concurrency/channel.hpp
overloads:
  sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> & operator=(const sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &):
    annotation:
      - delete
    arguments:
      - description: __OPTIONAL__
        name: x
        type: const sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> & operator=(const sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> & x)
  sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> & operator=(sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &&):
    annotation:
      - default
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &&
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> & operator=(sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &&)
---
