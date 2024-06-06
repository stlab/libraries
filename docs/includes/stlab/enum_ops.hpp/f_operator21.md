---
layout: function
title: operator!
hyde:
  owner: sean-parent
  brief: __MISSING__
  tags:
    - function
  defined_in_file: stlab/enum_ops.hpp
  overloads:
    "template <class T>\nconstexpr auto operator!(T) -> stlab::implementation::enable_if_bitmask_or_arithmetic<T, bool>":
      arguments:
        - description: __OPTIONAL__
          name: lhs
          type: T
      description: __OPTIONAL__
      return: __OPTIONAL__
      signature_with_names: "template <class T>\nconstexpr auto operator!(T lhs) -> stlab::implementation::enable_if_bitmask_or_arithmetic<T, bool>"
---
