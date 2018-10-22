---
layout: method
title: unique_instance
owner: sparent
brief: Forwarding routine for `unique`.
tags:
  - method
defined-in-file: stlab/copy_on_write.hpp
overloads:
  _Bool unique_instance() const:
    annotation:
      - public
      - deprecated ()
    description: Singular variant
    return: "`true` iff the object instance reference count is one. `false` otherwise."
    signature_with_names: _Bool unique_instance() const
---
