---
layout: class
title: regular
owner: sean-parent
brief: A class used to illustrate compiler and object behavior
example: regular_example.cpp
tags:
  - class
defined_in_file: test/model.hpp
declaration: "\nstruct stlab::regular;"
fields:
  _x:
    description: __MISSING__
    type: int
namespace:
  - stlab
  - v1
---

`regular` will write to `std::cout` when the following events happen to an instance:

- ctor
- dtor
- copy-ctor
- move-ctor
- copy-assign
- move-assign
- operator <
- swap