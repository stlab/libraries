---
layout: function
title: operator==
hyde:
  owner: sean-parent
  brief: __INLINED__
  tags:
    - function
  inline:
    brief:
      - "***********************************************************************************************"
  defined_in_file: stlab/enum_ops.hpp
  overloads:
    "template <class T>\nconstexpr auto operator==(T, std::nullptr_t) -> stlab::implementation::enable_if_bitmask_or_arithmetic<T, bool>":
      arguments:
        - description: __OPTIONAL__
          name: lhs
          type: T
        - description: __OPTIONAL__
          name: unnamed-1
          type: std::nullptr_t
          unnamed: true
      description: __INLINED__
      inline:
        description:
          - "***********************************************************************************************"
      return: __OPTIONAL__
      signature_with_names: "template <class T>\nconstexpr auto operator==(T lhs, std::nullptr_t) -> stlab::implementation::enable_if_bitmask_or_arithmetic<T, bool>"
    "template <class T>\nconstexpr auto operator==(std::nullptr_t, T) -> stlab::implementation::enable_if_bitmask_or_arithmetic<T, bool>":
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: std::nullptr_t
          unnamed: true
        - description: __OPTIONAL__
          name: rhs
          type: T
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: "template <class T>\nconstexpr auto operator==(std::nullptr_t, T rhs) -> stlab::implementation::enable_if_bitmask_or_arithmetic<T, bool>"
---
