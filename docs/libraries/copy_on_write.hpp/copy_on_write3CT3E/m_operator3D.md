---
layout: method
title: operator=
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: copy_on_write.hpp
overloads:
  auto operator=(const copy_on_write<T> &) -> copy_on_write<T> &:
    arguments:
      - description: __OPTIONAL__
        name: x
        type: const copy_on_write<T> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: auto operator=(const copy_on_write<T> & x) -> copy_on_write<T> &
  auto operator=(copy_on_write<T> &&) -> copy_on_write<T> &:
    arguments:
      - description: __OPTIONAL__
        name: x
        type: copy_on_write<T> &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: auto operator=(copy_on_write<T> && x) -> copy_on_write<T> &
  "template <class U>\nauto operator=(U &&) -> disable_copy_assign<U>":
    arguments:
      - description: __OPTIONAL__
        name: x
        type: U &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class U>\nauto operator=(U && x) -> disable_copy_assign<U>"
---
