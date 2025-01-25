---
layout: method
title: future<T, typename enable_if<smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type>
hyde:
  owner: sean-parent
  brief: __OPTIONAL__
  tags:
    - method
  defined_in_file: stlab/concurrency/future.hpp
  is_ctor: true
  overloads:
    future<T, typename enable_if<smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type>():
      annotation:
        - defaulted
      description: __OPTIONAL__
      signature_with_names: future<T, typename enable_if<smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type>()
---
