---
layout: method
title: operator=
hyde:
  owner: sean-parent
  brief: __OPTIONAL__
  tags:
    - method
  defined_in_file: concurrency/channel.hpp
  overloads:
    sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> & operator=(const sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &):
      annotation:
        - deleted
      arguments:
        - description: __OPTIONAL__
          name: x
          type: const sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &
      description: __OPTIONAL__
      return: __OPTIONAL__
      signature_with_names: sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> & operator=(const sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> & x)
    sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> & operator=(sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &&):
      annotation:
        - defaulted
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &&
          unnamed: true
      description: __OPTIONAL__
      return: __OPTIONAL__
      signature_with_names: sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> & operator=(sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &&)
---
