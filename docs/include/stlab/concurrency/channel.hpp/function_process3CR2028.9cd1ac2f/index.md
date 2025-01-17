---
layout: class
title: function_process<R (Args...)>
hyde:
  owner: sean-parent
  brief: __MISSING__
  tags:
    - class
  defined_in_file: stlab/concurrency/channel.hpp
  declaration: "\nstruct stlab::function_process;"
  dtor: unspecified
  typedefs:
    signature:
      definition: R (Args...)
      description: __MISSING__
  fields:
    _bound:
      description: __MISSING__
      type: std::function<R ()>
    _done:
      description: __MISSING__
      type: bool
    _f:
      description: __MISSING__
      type: std::function<R (Args...)>
  namespace:
    - stlab
    - v3
---
