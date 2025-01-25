---
layout: function
title: operator!=
hyde:
  owner: sean-parent
  brief: __MISSING__
  tags:
    - function
  defined_in_file: stlab/test/model.hpp
  overloads:
    auto operator!=(const stlab::annotate &, const stlab::annotate &) -> bool:
      arguments:
        - description: __OPTIONAL__
          name: x
          type: const stlab::annotate &
        - description: __OPTIONAL__
          name: y
          type: const stlab::annotate &
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: auto operator!=(const stlab::annotate & x, const stlab::annotate & y) -> bool
    auto operator!=(const stlab::regular &, const stlab::regular &) -> bool:
      arguments:
        - description: __OPTIONAL__
          name: lhs
          type: const stlab::regular &
        - description: __OPTIONAL__
          name: rhs
          type: const stlab::regular &
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: auto operator!=(const stlab::regular & lhs, const stlab::regular & rhs) -> bool
  namespace:
    - stlab
    - v3
---
