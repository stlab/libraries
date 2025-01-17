---
layout: method
title: swap
hyde:
  owner: sean-parent
  brief: __MISSING__
  tags:
    - method
  defined_in_file: stlab/concurrency/future.hpp
  overloads:
    void swap(future<T, typename enable_if<smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> &):
      arguments:
        - description: __OPTIONAL__
          name: x
          type: future<T, typename enable_if<smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> &
      description: __OPTIONAL__
      return: __OPTIONAL__
      signature_with_names: void swap(future<T, typename enable_if<smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> & x)
---
