---
layout: method
title: operator=
hyde:
  owner: __OPTIONAL__
  brief: __OPTIONAL__
  tags:
    - method
  defined_in_file: concurrency/future.hpp
  overloads:
    future<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> & operator=(const future<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &):
      annotation:
        - deleted
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: const future<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &
          unnamed: true
      description: __OPTIONAL__
      return: __OPTIONAL__
      signature_with_names: future<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> & operator=(const future<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &)
    future<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> & operator=(future<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &&):
      annotation:
        - defaulted
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: future<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &&
          unnamed: true
      description: __OPTIONAL__
      return: __OPTIONAL__
      signature_with_names: future<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> & operator=(future<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &&)
---
