---
layout: function
title: reverse_append
hyde:
  owner: sean-parent
  brief: __INLINED__
  tags:
    - function
  inline:
    brief: _multiple descriptions_
  defined_in_file: stlab/algorithm/reverse.hpp
  overloads:
    "template <typename I>\nauto reverse_append(I, I, I) -> I":
      arguments:
        - description: __OPTIONAL__
          name: first
          type: I
        - description: __OPTIONAL__
          name: last
          type: I
        - description: __OPTIONAL__
          name: end
          type: I
      description: "`I` is a model of `forward_node_iterator`.\n"
      inline:
        description:
          - Reverses the range `[first, last)` and appends `end`. Returns the beginning of the reversed range such that `[result, end)` is a valid range.
      return: __OPTIONAL__
      signature_with_names: "template <typename I>\nauto reverse_append(I first, I last, I end) -> I"
    "template <typename R, typename I>\nauto reverse_append(R &, I) -> I":
      arguments:
        - description: __OPTIONAL__
          name: range
          type: R &
        - description: __OPTIONAL__
          name: end
          type: I
      description: "`R` is a model of `forward_node_range`. `I` is a model of `forward_node_iterator`.\n"
      inline:
        description:
          - Reverses `range` and appends `end`.  Returns the beginning of the reversed range such that `[result, end)` is a valid range.
      return: __OPTIONAL__
      signature_with_names: "template <typename R, typename I>\nauto reverse_append(R & range, I end) -> I"
  namespace:
    - stlab
    - unsafe
---
