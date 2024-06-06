---
layout: function
title: apply_ignore_placeholders
hyde:
  owner: sean-parent
  brief: __INLINED__
  tags:
    - function
  inline:
    brief:
      - "***********************************************************************************************"
      - apply the tuple `t`as arguments to the function `f`. Placeholders are ignored.
  defined_in_file: stlab/concurrency/tuple_algorithm.hpp
  overloads:
    "template <class F, class Tuple>\nconstexpr auto apply_ignore_placeholders(F &&, Tuple &&) -> decltype(auto)":
      arguments:
        - description: __OPTIONAL__
          name: f
          type: F &&
        - description: __OPTIONAL__
          name: t
          type: Tuple &&
      description: __INLINED__
      inline:
        description:
          - "***********************************************************************************************"
          - apply the tuple `t`as arguments to the function `f`. Placeholders are ignored.
      return: __OPTIONAL__
      signature_with_names: "template <class F, class Tuple>\nconstexpr auto apply_ignore_placeholders(F && f, Tuple && t) -> decltype(auto)"
  namespace:
    - stlab
    - v3
---
