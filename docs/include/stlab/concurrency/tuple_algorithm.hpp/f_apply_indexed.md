---
layout: function
title: apply_indexed
hyde:
  owner: sean-parent
  brief: __INLINED__
  tags:
    - function
  inline:
    brief:
      - "***********************************************************************************************"
      - apply_indexed applies the tuple `t` as arguments to the function `f` using the index sequence `Seq` to select the arguments.
  defined_in_file: stlab/concurrency/tuple_algorithm.hpp
  overloads:
    "template <class Seq, class F, class Tuple>\nconstexpr auto apply_indexed(F &&, Tuple &&) -> decltype(auto)":
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
          - apply_indexed applies the tuple `t` as arguments to the function `f` using the index sequence `Seq` to select the arguments.
      return: __OPTIONAL__
      signature_with_names: "template <class Seq, class F, class Tuple>\nconstexpr auto apply_indexed(F && f, Tuple && t) -> decltype(auto)"
  namespace:
    - stlab
    - v3
---
