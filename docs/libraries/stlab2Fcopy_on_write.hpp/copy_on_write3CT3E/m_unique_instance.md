---
layout: method
title: unique_instance
owner: sean-parent
brief: Forwarding routine for `unique`.
tags:
  - method
defined-in-file: stlab/copy_on_write.hpp
overloads:
  bool unique_instance() const:
    annotation:
      - deprecated ()
    description: Singular variant
    return: "`true` iff the object instance reference count is one. `false` otherwise."
    signature_with_names: bool unique_instance() const
---
