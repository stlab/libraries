---
layout: function
title: operator<=
owner: sean-parent
brief: Less-than-or-equal operator
tags:
  - function
defined-in-file: stlab/copy_on_write.hpp
overloads:
  bool operator<=(const copy_on_write<T> &, const copy_on_write<T> &):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: const copy_on_write<T> &
      - description: __OPTIONAL__
        name: y
        type: const copy_on_write<T> &
    description: Operator against same `copy_on_write` type
    return: __OPTIONAL__
    signature_with_names: bool operator<=(const copy_on_write<T> & x, const copy_on_write<T> & y)
  bool operator<=(const copy_on_write<T> &, const stlab::copy_on_write::element_type &):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: const copy_on_write<T> &
      - description: __OPTIONAL__
        name: y
        type: const stlab::copy_on_write::element_type &
    description: Operator against `element_type` as rhs
    return: __OPTIONAL__
    signature_with_names: bool operator<=(const copy_on_write<T> & x, const stlab::copy_on_write::element_type & y)
  bool operator<=(const stlab::copy_on_write::element_type &, const copy_on_write<T> &):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: const stlab::copy_on_write::element_type &
      - description: __OPTIONAL__
        name: y
        type: const copy_on_write<T> &
    description: Operator against `element_type` as lhs
    return: __OPTIONAL__
    signature_with_names: bool operator<=(const stlab::copy_on_write::element_type & x, const copy_on_write<T> & y)
---
