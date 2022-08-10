---
layout: method
title: unique
owner: sean-parent
brief: Returns if the object instance reference count is one
tags:
  - method
defined_in_file: copy_on_write.hpp
overloads:
  bool unique() const:
    description: Singular variant
    return: "`true` iff the object instance reference count is one. `false` otherwise."
    signature_with_names: bool unique() const
---

This is useful to determine if writing will cause a copy.
