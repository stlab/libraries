---
layout: method
title: identity
owner: sparent
brief: Used to see if two copy_on_write items refer to the same instance.
tags:
  - method
defined-in-file: stlab/copy_on_write.hpp
overloads:
  _Bool identity(const copy_on_write<T> &) const:
    annotation:
      - public
    arguments:
      - description: __OPTIONAL__
        name: x
        type: const copy_on_write<T> &
    description: Singular variant
    return: "`true` iff the underlying object instance is shared by both objects. `false` otherwise."
    signature_with_names: _Bool identity(const copy_on_write<T> & x) const
---
