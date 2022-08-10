---
layout: function
title: operator!=
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: test/model.hpp
overloads:
  bool operator!=(const stlab::annotate &, const stlab::annotate &):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: const stlab::annotate &
      - description: __OPTIONAL__
        name: y
        type: const stlab::annotate &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: bool operator!=(const stlab::annotate & x, const stlab::annotate & y)
  bool operator!=(const stlab::regular &, const stlab::regular &):
    arguments:
      - description: __OPTIONAL__
        name: lhs
        type: const stlab::regular &
      - description: __OPTIONAL__
        name: rhs
        type: const stlab::regular &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: bool operator!=(const stlab::regular & lhs, const stlab::regular & rhs)
namespace:
  - stlab
  - v1
---
