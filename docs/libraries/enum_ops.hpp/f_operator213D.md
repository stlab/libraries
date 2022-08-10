---
layout: function
title: operator!=
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: enum_ops.hpp
overloads:
  "template <class T>\nconstexpr auto operator!=(T, std::nullptr_t) -> stlab::implementation::enable_if_bitmask_or_arithmetic<T, bool>":
    arguments:
      - description: __OPTIONAL__
        name: lhs
        type: T
      - description: __OPTIONAL__
        name: rhs
        type: std::nullptr_t
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class T>\nconstexpr auto operator!=(T lhs, std::nullptr_t rhs) -> stlab::implementation::enable_if_bitmask_or_arithmetic<T, bool>"
  "template <class T>\nconstexpr auto operator!=(std::nullptr_t, T) -> stlab::implementation::enable_if_bitmask_or_arithmetic<T, bool>":
    arguments:
      - description: __OPTIONAL__
        name: lhs
        type: std::nullptr_t
      - description: __OPTIONAL__
        name: rhs
        type: T
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class T>\nconstexpr auto operator!=(std::nullptr_t lhs, T rhs) -> stlab::implementation::enable_if_bitmask_or_arithmetic<T, bool>"
---
