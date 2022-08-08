---
layout: class
title: copy_on_write<T>
owner: sean-parent
brief: A copy-on-write wrapper for any type that models `RegularType`
tags:
  - class
defined_in_file: copy_on_write.hpp
declaration: "template <typename T>\nclass stlab::copy_on_write;"
typedefs:
  element_type:
    definition: T
    description: The type of value stored
  value_type:
    definition: T
    description: The type of value stored
fields:
  default_s:
    description: Shared, empty model instance
    type: stlab::copy_on_write::model
namespace:
  - stlab
---

Copy-on-write semantics allow for an object to be lazily copied - only creating a copy when 1) the value is modified, and 2) there is more than one reference to the value.

`copy_on_write` is thread safe, and supports types that model `MoveableType`.
