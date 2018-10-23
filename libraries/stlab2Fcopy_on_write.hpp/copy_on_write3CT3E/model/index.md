---
layout: class
title: stlab::copy_on_write<T>::model
owner: sparent
brief: Underlying model instance
tags:
  - class
defined-in-file: stlab/copy_on_write.hpp
declaration: "\nstruct stlab::copy_on_write<T>::model;"
annotation:
  - private
dtor: unspecified
fields:
  _count:
    annotation:
      - public
    description: Reference count
    type: std::atomic<std::size_t>
  _value:
    annotation:
      - public
    description: Value instance
    type: T
namespace:
  - stlab
---
