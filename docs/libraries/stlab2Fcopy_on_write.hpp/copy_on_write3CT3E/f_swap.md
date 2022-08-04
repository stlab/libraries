---
layout: function
title: swap
owner: sean-parent
brief: Swap specialization
tags:
  - function
defined-in-file: stlab/copy_on_write.hpp
overloads:
  void swap(copy_on_write<T> &, copy_on_write<T> &):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: copy_on_write<T> &
      - description: __OPTIONAL__
        name: y
        type: copy_on_write<T> &
    description: Singular variant
    return: __OPTIONAL__
    signature_with_names: void swap(copy_on_write<T> & x, copy_on_write<T> & y)
---
