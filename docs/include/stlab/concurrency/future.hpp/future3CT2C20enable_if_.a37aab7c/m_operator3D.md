---
layout: method
title: operator=
hyde:
  owner: sean-parent
  brief: __OPTIONAL__
  tags:
    - method
  defined_in_file: stlab/concurrency/future.hpp
  overloads:
    auto operator=(const future<T, typename enable_if<!smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> &) -> future<T, typename enable_if<!smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> &:
      annotation:
        - deleted
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: const future<T, typename enable_if<!smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> &
          unnamed: true
      description: __OPTIONAL__
      return: __OPTIONAL__
      signature_with_names: auto operator=(const future<T, typename enable_if<!smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> &) -> future<T, typename enable_if<!smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> &
    auto operator=(future<T, typename enable_if<!smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> &&) -> future<T, typename enable_if<!smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> &:
      annotation:
        - defaulted
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: future<T, typename enable_if<!smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> &&
          unnamed: true
      description: __OPTIONAL__
      return: __OPTIONAL__
      signature_with_names: auto operator=(future<T, typename enable_if<!smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> &&) -> future<T, typename enable_if<!smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> &
---
