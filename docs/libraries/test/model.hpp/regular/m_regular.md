---
layout: method
title: regular
owner: sean-parent
brief: Constructs a regular
tags:
  - method
defined_in_file: test/model.hpp
is_ctor: true
overloads:
  explicit regular(int):
    arguments:
      - description: The value the instance should take
        name: x
        type: int
    description: Constructs the regular instance
    return: __OPTIONAL__
    signature_with_names: explicit regular(int x)
  regular(const stlab::regular &):
    arguments:
      - description: __OPTIONAL__
        name: rhs
        type: const stlab::regular &
    description: Constructs the regular instance
    return: __OPTIONAL__
    signature_with_names: regular(const stlab::regular & rhs)
  regular(stlab::regular &&):
    arguments:
      - description: __OPTIONAL__
        name: rhs
        type: stlab::regular &&
    description: Constructs the regular instance
    return: __OPTIONAL__
    signature_with_names: regular(stlab::regular && rhs)
---
