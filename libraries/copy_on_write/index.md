---
layout: class
title: stlab::copy_on_write
tags: [library]
name: stlab::copy_on_write
pure-name: copy_on_write
defined-in-header: stlab/copy_on_write.hpp
dtor: default
annotation: template class
declaration: |
  template <typename T>
  class copy_on_write
brief: A copy-on-write wrapper for any type that models `RegularType`.
description: |
    Copy-on-write sematics allow for an object to be lazily copied - only creating a copy when 1) the value is modified, and 2) there is more than one reference to the value.

    `copy_on_write` is thread safe, and supports types that model `MoveableType`.
member-types:
  - type: value_type
    definition: T
    note: The type of value stored
    annotation: |
        [[deprecated]]
  - type: element_type
    definition: T
    note: The type of value stored
---
