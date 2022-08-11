---
layout: function
title: operator~
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: enum_ops.hpp
overloads:
  "template <class T>\nconstexpr auto operator~(T) -> std::enable_if_t<stlab::implementation::has_enabled_bitmask<T>, T>":
    arguments:
      - description: __OPTIONAL__
        name: a
        type: T
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <class T>\nconstexpr auto operator~(T a) -> std::enable_if_t<stlab::implementation::has_enabled_bitmask<T>, T>"
---
