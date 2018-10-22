---
layout: method
title: unique
owner: sparent
brief: Returns if the object instance reference count is one
tags:
  - method
defined-in-file: stlab/copy_on_write.hpp
overloads:
  _Bool unique() const:
    annotation:
      - public
    description: Singular variant
    return: "`true` iff the object instance reference count is one. `false` otherwise."
    signature_with_names: _Bool unique() const
---

This is useful to determine if writing will cause a copy.
