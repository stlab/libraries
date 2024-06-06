---
layout: class
title: void_to_monostate<T>
hyde:
  owner: __MISSING__
  brief: __MISSING__
  tags:
    - class
  inline:
    description:
      - "REVISIT (sean-parent) : As a typedef, this generates file names in the hyde documentation that are too long for windows. Moving to a class for now, but I may also change how this is used and have a single future < > class with conditional members for easier documentation."
  defined_in_file: stlab/concurrency/future.hpp
  declaration: "template <class T>\nstruct stlab::void_to_monostate;"
  ctor: unspecified
  dtor: unspecified
  typedefs:
    type:
      definition: std::conditional_t<std::is_void_v<T>, std::monostate, T>
      description: __MISSING__
  namespace:
    - stlab
    - v3
---
