---
layout: function
title: operator==
hyde:
  owner: sean-parent
  brief: __MISSING__
  tags:
    - function
  defined_in_file: stlab/concurrency/channel.hpp
  overloads:
    auto operator==(const receiver<T> &, const receiver<T> &) -> bool:
      arguments:
        - description: __OPTIONAL__
          name: x
          type: const receiver<T> &
        - description: __OPTIONAL__
          name: y
          type: const receiver<T> &
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: auto operator==(const receiver<T> & x, const receiver<T> & y) -> bool
    auto operator==(const sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &, const sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &) -> bool:
      arguments:
        - description: __OPTIONAL__
          name: x
          type: const sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &
        - description: __OPTIONAL__
          name: y
          type: const sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> &
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: auto operator==(const sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> & x, const sender<T, typename enable_if<!smart_is_copy_constructible_v<T>, void>::type> & y) -> bool
    auto operator==(const sender<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> &, const sender<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> &) -> bool:
      arguments:
        - description: __OPTIONAL__
          name: x
          type: const sender<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> &
        - description: __OPTIONAL__
          name: y
          type: const sender<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> &
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: auto operator==(const sender<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> & x, const sender<T, typename enable_if<smart_is_copy_constructible_v<T>, void>::type> & y) -> bool
  namespace:
    - stlab
    - v3
---
