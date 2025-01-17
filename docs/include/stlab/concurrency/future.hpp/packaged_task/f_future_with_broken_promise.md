---
layout: function
title: future_with_broken_promise
hyde:
  owner: sean-parent
  brief: __MISSING__
  tags:
    - function
  defined_in_file: stlab/concurrency/future.hpp
  overloads:
    "template <class T, class E>\nauto future_with_broken_promise(E) -> detail::reduced_t<T>":
      arguments:
        - description: __OPTIONAL__
          name: executor
          type: E
          unnamed: true
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: "template <class T, class E>\nauto future_with_broken_promise(E executor) -> detail::reduced_t<T>"
    "template <class U, class E>\nauto future_with_broken_promise(E) -> detail::reduced_t<U>":
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: E
          unnamed: true
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: "template <class U, class E>\nauto future_with_broken_promise(E) -> detail::reduced_t<U>"
  namespace:
    - stlab
    - v3
---
