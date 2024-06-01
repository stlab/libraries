---
layout: method
title: unique
hyde:
  owner: sean-parent
  brief: Returns if the object instance reference count is one
  tags:
    - method
  defined_in_file: stlab/copy_on_write.hpp
  overloads:
    auto unique() const -> bool:
      description: __OPTIONAL__
      return: __OPTIONAL__
      signature_with_names: auto unique() const -> bool
---

This is useful to determine if writing will cause a copy.
