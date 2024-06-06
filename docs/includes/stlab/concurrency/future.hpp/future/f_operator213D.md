---
layout: function
title: operator!=
hyde:
  owner: sean-parent
  brief: __MISSING__
  tags:
    - function
  defined_in_file: stlab/concurrency/future.hpp
  overloads:
    auto operator!=(const future<T, typename enable_if<!smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> &, const future<T, typename enable_if<!smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> &) -> bool:
      arguments:
        - description: __OPTIONAL__
          name: x
          type: const future<T, typename enable_if<!smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> &
        - description: __OPTIONAL__
          name: y
          type: const future<T, typename enable_if<!smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> &
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: auto operator!=(const future<T, typename enable_if<!smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> & x, const future<T, typename enable_if<!smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> & y) -> bool
    auto operator!=(const future<T, typename enable_if<smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> &, const future<T, typename enable_if<smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> &) -> bool:
      arguments:
        - description: __OPTIONAL__
          name: x
          type: const future<T, typename enable_if<smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> &
        - description: __OPTIONAL__
          name: y
          type: const future<T, typename enable_if<smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> &
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: auto operator!=(const future<T, typename enable_if<smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> & x, const future<T, typename enable_if<smart_is_copy_constructible_v<typename void_to_monostate<T>::type>, void>::type> & y) -> bool
    auto operator!=(const stlab::future<void> &, const stlab::future<void> &) -> bool:
      arguments:
        - description: __OPTIONAL__
          name: x
          type: const stlab::future<void> &
        - description: __OPTIONAL__
          name: y
          type: const stlab::future<void> &
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: auto operator!=(const stlab::future<void> & x, const stlab::future<void> & y) -> bool
  namespace:
    - stlab
    - v3
---
