---
layout: function
title: reverse_until
hyde:
  owner: sean-parent
  brief: __INLINED__
  tags:
    - function
  inline:
    brief:
      - Reverses the range `[f, l)` until `m` is reached. Returns a range [a, b) of the un-reversed subrange such that either `a == m` or `b == m`.
  defined_in_file: stlab/algorithm/reverse.hpp
  overloads:
    "template <typename I>\nauto reverse_until(I, I, I) -> std::pair<I, I>":
      arguments:
        - description: __OPTIONAL__
          name: f
          type: I
        - description: __OPTIONAL__
          name: m
          type: I
        - description: __OPTIONAL__
          name: l
          type: I
      description: "`I` is a model of `std::bidirectional_iterator`.\n"
      inline:
        description:
          - Reverses the range `[f, l)` until `m` is reached. Returns a range [a, b) of the un-reversed subrange such that either `a == m` or `b == m`.
      return: __OPTIONAL__
      signature_with_names: "template <typename I>\nauto reverse_until(I f, I m, I l) -> std::pair<I, I>"
  namespace:
    - stlab
---
