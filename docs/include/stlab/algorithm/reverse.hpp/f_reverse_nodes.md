---
layout: function
title: reverse_nodes
hyde:
  owner: sean-parent
  brief: __INLINED__
  tags:
    - function
  inline:
    brief: _multiple descriptions_
  defined_in_file: stlab/algorithm/reverse.hpp
  overloads:
    "template <typename I>\nauto reverse_nodes(I, I) -> I":
      arguments:
        - description: __OPTIONAL__
          name: first
          type: I
        - description: __OPTIONAL__
          name: last
          type: I
      description: __INLINED__
      inline:
        description:
          - Reverses the range `[first, last)` and returns the beginning of the reversed range such that `[result, last)` is a valid range.
      return: __OPTIONAL__
      signature_with_names: "template <typename I>\nauto reverse_nodes(I first, I last) -> I"
    "template <typename R>\nauto reverse_nodes(R &) -> typename R::iterator":
      arguments:
        - description: __OPTIONAL__
          name: range
          type: R &
      description: __INLINED__
      inline:
        description:
          - Reverses the range `range` and returns the beginning of the reversed range such that `[result, last)` is a valid range.
      return: __OPTIONAL__
      signature_with_names: "template <typename R>\nauto reverse_nodes(R & range) -> typename R::iterator"
  namespace:
    - stlab
    - unsafe
---
