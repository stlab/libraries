---
layout: method
title: operator=
hyde:
  owner: FelixPetriconi
  brief: __MISSING__
  tags:
    - method
  defined_in_file: stlab/concurrency/channel.hpp
  overloads:
    auto operator=(const receiver<T> &) -> receiver<T> &:
      arguments:
        - description: __OPTIONAL__
          name: x
          type: const receiver<T> &
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: auto operator=(const receiver<T> & x) -> receiver<T> &
    auto operator=(receiver<T> &&) -> receiver<T> &:
      annotation:
        - defaulted
      arguments:
        - description: __OPTIONAL__
          name: x
          type: receiver<T> &&
      description: __OPTIONAL__
      return: __OPTIONAL__
      signature_with_names: auto operator=(receiver<T> && x) -> receiver<T> &
---
