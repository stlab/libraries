---
layout: method
title: future<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type>
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: concurrency/future.hpp
is_ctor: true
overloads:
  future<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type>():
    annotation:
      - default
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: future<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type>()
  future<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type>(future<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &&):
    annotation:
      - default
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: future<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &&
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: future<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type>(future<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &&)
---
