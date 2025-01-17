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
    auto operator=(const stlab::annotate &) -> stlab::annotate &:
      arguments:
        - description: __OPTIONAL__
          name: x
          type: const stlab::annotate &
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: auto operator=(const stlab::annotate & x) -> stlab::annotate &
    auto operator=(stlab::annotate &&) -> stlab::annotate &:
      arguments:
        - description: __OPTIONAL__
          name: x
          type: stlab::annotate &&
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: auto operator=(stlab::annotate && x) -> stlab::annotate &
---
