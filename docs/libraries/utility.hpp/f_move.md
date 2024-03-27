---
layout: function
title: move
hyde:
  owner: dhaibatc
  brief: __INLINED__
  tags:
    - function
  inline:
    brief:
      - A standard move implementation but with a compile-time check for const types.
  defined_in_file: utility.hpp
  overloads:
    "template <class T>\nconstexpr std::remove_reference_t<T> && move(T &&)":
      arguments:
        - description: __OPTIONAL__
          name: t
          type: T &&
      description: __INLINED__
      inline:
        description:
          - A standard move implementation but with a compile-time check for const types.
      return: __OPTIONAL__
      signature_with_names: "template <class T>\nconstexpr std::remove_reference_t<T> && move(T && t)"
  namespace:
    - stlab
    - v2
---
