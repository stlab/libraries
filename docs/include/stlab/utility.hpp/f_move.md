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
  defined_in_file: stlab/utility.hpp
  overloads:
    "template <class T>\nconstexpr auto move(T &&) -> std::remove_reference_t<T> &&":
      arguments:
        - description: __OPTIONAL__
          name: t
          type: T &&
      description: __INLINED__
      inline:
        description:
          - A standard move implementation but with a compile-time check for const types.
      return: __OPTIONAL__
      signature_with_names: "template <class T>\nconstexpr auto move(T && t) -> std::remove_reference_t<T> &&"
  namespace:
    - stlab
    - v3
---
