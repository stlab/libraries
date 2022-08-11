---
layout: function
title: operator>
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: copy_on_write.hpp
overloads:
  bool operator>(const copy_on_write<T> &, const copy_on_write<T> &):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: const copy_on_write<T> &
      - description: __OPTIONAL__
        name: y
        type: const copy_on_write<T> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: bool operator>(const copy_on_write<T> & x, const copy_on_write<T> & y)
  bool operator>(const copy_on_write<T> &, const stlab::copy_on_write::element_type &):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: const copy_on_write<T> &
      - description: __OPTIONAL__
        name: y
        type: const stlab::copy_on_write::element_type &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: bool operator>(const copy_on_write<T> & x, const stlab::copy_on_write::element_type & y)
  bool operator>(const stlab::copy_on_write::element_type &, const copy_on_write<T> &):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: const stlab::copy_on_write::element_type &
      - description: __OPTIONAL__
        name: y
        type: const copy_on_write<T> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: bool operator>(const stlab::copy_on_write::element_type & x, const copy_on_write<T> & y)
namespace:
  - stlab
---
