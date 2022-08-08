---
layout: class
title: annotate
owner: sean-parent
brief: A class used to illustrate compiler and object behavior
example: annotate_rvo_example.cpp
tags:
  - class
defined_in_file: test/model.hpp
declaration: "\nstruct stlab::annotate;"
fields:
  _counters:
    description: __MISSING__
    type: stlab::annotate_counters *
namespace:
  - stlab
  - v1
---

`annotate` will increase a passed operation counter when one of these operations happen to an instance:

- dtor
- copy-ctor
- move-ctor
- copy-assign
- move-assign
- swap

In addition, `operator==` is always `true`, and `operator!=` is always `false`.