---
layout: function
title: swap
hyde:
  owner: sean-parent
  brief: __MISSING__
  tags:
    - function
  defined_in_file: stlab/concurrency/future.hpp
  overloads:
    void swap(future<T, typename enable_if<!smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> &, future<T, typename enable_if<!smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> &):
      arguments:
        - description: __OPTIONAL__
          name: x
          type: future<T, typename enable_if<!smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> &
        - description: __OPTIONAL__
          name: y
          type: future<T, typename enable_if<!smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> &
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: void swap(future<T, typename enable_if<!smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> & x, future<T, typename enable_if<!smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> & y)
    void swap(future<T, typename enable_if<smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> &, future<T, typename enable_if<smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> &):
      arguments:
        - description: __OPTIONAL__
          name: x
          type: future<T, typename enable_if<smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> &
        - description: __OPTIONAL__
          name: y
          type: future<T, typename enable_if<smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> &
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: void swap(future<T, typename enable_if<smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> & x, future<T, typename enable_if<smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> & y)
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
    - v3
---
