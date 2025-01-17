---
layout: class
title: detector<Default, void_t<Op<Args...>>, Op, Args...>
hyde:
  owner: sean-parent
  brief: __MISSING__
  tags:
    - class
  inline:
    description:
      - "the specialization recognizes and handles only types supporting Op:"
  defined_in_file: stlab/concurrency/traits.hpp
  declaration: "\nstruct stlab::detector;"
  ctor: unspecified
  dtor: unspecified
  typedefs:
    type:
      definition: Op<Args...>
      description: __MISSING__
    value_t:
      definition: std::true_type
      description: __MISSING__
  namespace:
    - stlab
    - v3
---
