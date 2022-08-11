---
layout: function
title: swap
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: test/model.hpp
overloads:
  void swap(stlab::annotate &, stlab::annotate &):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: stlab::annotate &
      - description: __OPTIONAL__
        name: y
        type: stlab::annotate &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: void swap(stlab::annotate & x, stlab::annotate & y)
  void swap(stlab::regular &, stlab::regular &):
    arguments:
      - description: __OPTIONAL__
        name: lhs
        type: stlab::regular &
      - description: __OPTIONAL__
        name: rhs
        type: stlab::regular &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: void swap(stlab::regular & lhs, stlab::regular & rhs)
namespace:
  - stlab
  - v1
---
