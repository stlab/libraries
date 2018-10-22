---
layout: method
title: operator=
owner: sparent
brief: Assignment operator
tags:
  - method
defined-in-file: stlab/copy_on_write.hpp
overloads:
  auto operator=(const copy_on_write<T> &) -> copy_on_write<T> &:
    annotation:
      - public
    arguments:
      - description: __OPTIONAL__
        name: x
        type: const copy_on_write<T> &
    description: Copy assignment
    return: __OPTIONAL__
    signature_with_names: auto operator=(const copy_on_write<T> & x) -> copy_on_write<T> &
  auto operator=(copy_on_write<T> &&) -> copy_on_write<T> &:
    annotation:
      - public
    arguments:
      - description: __OPTIONAL__
        name: x
        type: copy_on_write<T> &&
    description: Move assignment
    return: __OPTIONAL__
    signature_with_names: auto operator=(copy_on_write<T> && x) -> copy_on_write<T> &
  template <class U> auto operator=(U &&) -> disable_copy_assign<U>:
    annotation:
      - public
    arguments:
      - description: __OPTIONAL__
        name: x
        type: U &&
    description: Copy assignment from non-`T`-type
    return: __OPTIONAL__
    signature_with_names: template <class U> auto operator=(U && x) -> disable_copy_assign<U>
---
