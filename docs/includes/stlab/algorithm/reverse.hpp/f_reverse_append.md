---
layout: function
title: reverse_append
hyde:
  owner: __MISSING__
  brief: __INLINED__
  tags:
    - function
  inline:
    brief:
      - "***********************************************************************************************"
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
          name: result
          type: I
      description: __INLINED__
      inline:
        description:
          - "***********************************************************************************************"
      return: __OPTIONAL__
      signature_with_names: "template <typename I>\nauto reverse_append(I first, I last, I result) -> I"
    "template <typename R, typename I>\nauto reverse_append(R &, I) -> I":
      arguments:
        - description: __OPTIONAL__
          name: range
          type: R &
        - description: __OPTIONAL__
          name: result
          type: I
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: "template <typename R, typename I>\nauto reverse_append(R & range, I result) -> I"
  namespace:
    - stlab
    - unsafe
---
