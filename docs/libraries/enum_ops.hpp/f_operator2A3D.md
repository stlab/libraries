---
layout: function
title: operator*=
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: enum_ops.hpp
overloads:
  "template <class T, class U>\nconstexpr auto operator*=(T &, U) -> std::enable_if_t<stlab::implementation::has_enabled_arithmetic<T> && stlab::implementation::is_convertible_to_underlying<U, T>::value, T &>":
    arguments:
      - description: __OPTIONAL__
        name: lhs
        type: T &
      - description: __OPTIONAL__
        name: rhs
        type: U
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <class T, class U>\nconstexpr auto operator*=(T & lhs, U rhs) -> std::enable_if_t<stlab::implementation::has_enabled_arithmetic<T> && stlab::implementation::is_convertible_to_underlying<U, T>::value, T &>"
---
