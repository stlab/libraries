---
layout: method
title: operator=
hyde:
  owner: sean-parent
  brief: __MISSING__
  tags:
    - method
  defined_in_file: stlab/test/model.hpp
  overloads:
    auto operator=(const stlab::regular &) -> stlab::regular &:
      arguments:
        - description: __OPTIONAL__
          name: rhs
          type: const stlab::regular &
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: auto operator=(const stlab::regular & rhs) -> stlab::regular &
    auto operator=(stlab::regular &&) -> stlab::regular &:
      arguments:
        - description: __OPTIONAL__
          name: rhs
          type: stlab::regular &&
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: auto operator=(stlab::regular && rhs) -> stlab::regular &
---
