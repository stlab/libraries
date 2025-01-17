---
layout: method
title: operator=
hyde:
  owner: fosterbrereton
  brief: Assignment operator
  tags:
    - method
  defined_in_file: stlab/forest.hpp
  overloads:
    auto operator=(const forest<T> &) -> forest<T> &:
      arguments:
        - description: __OPTIONAL__
          name: x
          type: const forest<T> &
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: auto operator=(const forest<T> & x) -> forest<T> &
    auto operator=(forest<T> &&) -> forest<T> &:
      arguments:
        - description: __OPTIONAL__
          name: x
          type: forest<T> &&
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: auto operator=(forest<T> && x) -> forest<T> &
---
