---
layout: class
title: annotate
hyde:
  owner: sean-parent
  brief: A class used to illustrate compiler and object behavior
  tags:
    - class
  defined_in_file: stlab/test/model.hpp
  declaration: "\nstruct stlab::annotate;"
  fields:
    _counters:
      description: __MISSING__
      type: stlab::annotate_counters *
  namespace:
    - stlab
    - v3
---

`annotate` will increase a passed operation counter when one of these operations happen to an instance:

- dtor
- copy-ctor
- move-ctor
- copy-assign
- move-assign
- swap

In addition, `operator==` is always `true`, and `operator!=` is always `false`.
