---
layout: class
title: stlab::copy_on_write<T>
owner: sparent
brief: A copy-on-write wrapper for any type that models `RegularType`.
tags:
  - class
defined-in-file: stlab/copy_on_write.hpp
declaration: "template <typename T>\nclass stlab::copy_on_write<T>;"
typedefs:
  element_type:
    annotation:
      - public
    definition: T
    description: The type of value stored
  value_type:
    annotation:
      - public
    definition: T
    description: The type of value stored
fields:
  _self:
    annotation:
      - private
    description: Pimpl-idiom instance for this class
    type: stlab::copy_on_write::model *
  default_s:
    description: Shared, empty model instance.
    type: stlab::copy_on_write::model
namespace:
  - stlab
---

Copy-on-write semantics allow for an object to be lazily copied - only creating a copy when 1) the value is modified, and 2) there is more than one reference to the value.

`copy_on_write` is thread safe, and supports types that model `MoveableType`.
