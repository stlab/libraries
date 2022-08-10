---
layout: function
title: operator|
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: enum_ops.hpp
overloads:
  "template <class T>\nconstexpr auto operator|(T, T) -> std::enable_if_t<stlab::implementation::has_enabled_bitmask<T>, T>":
    arguments:
      - description: __OPTIONAL__
        name: lhs
        type: T
      - description: __OPTIONAL__
        name: rhs
        type: T
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <class T>\nconstexpr auto operator|(T lhs, T rhs) -> std::enable_if_t<stlab::implementation::has_enabled_bitmask<T>, T>"
---
