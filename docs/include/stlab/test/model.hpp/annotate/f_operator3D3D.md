---
layout: function
title: operator==
hyde:
  owner: sean-parent
  brief: Compares two objects for equality.
  tags:
    - function
  defined_in_file: stlab/test/model.hpp
  overloads:
    auto operator==(const stlab::annotate &, const stlab::annotate &) -> bool:
      arguments:
        - description: __OPTIONAL__
          name: x
          type: const stlab::annotate &
        - description: __OPTIONAL__
          name: y
          type: const stlab::annotate &
      description: Compares two `annotate` objects for equality.
      return: __OPTIONAL__
      signature_with_names: auto operator==(const stlab::annotate & x, const stlab::annotate & y) -> bool
    auto operator==(const stlab::move_only &, const stlab::move_only &) -> bool:
      arguments:
        - description: __OPTIONAL__
          name: a
          type: const stlab::move_only &
        - description: __OPTIONAL__
          name: b
          type: const stlab::move_only &
      description: Compares two `move_only` objects for equality.
      return: __OPTIONAL__
      signature_with_names: auto operator==(const stlab::move_only & a, const stlab::move_only & b) -> bool
    auto operator==(const stlab::regular &, const stlab::regular &) -> bool:
      arguments:
        - description: __OPTIONAL__
          name: lhs
          type: const stlab::regular &
        - description: __OPTIONAL__
          name: rhs
          type: const stlab::regular &
      description: Compares two `regular` objects for equality.
      return: __OPTIONAL__
      signature_with_names: auto operator==(const stlab::regular & lhs, const stlab::regular & rhs) -> bool
  namespace:
    - stlab
    - v2_1_0
---
