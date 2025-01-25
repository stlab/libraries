---
layout: function
title: reverse_fullorder_range
hyde:
  owner: fosterbrereton
  brief: Get a range of reverse iterators for a given range
  tags:
    - function
  inline:
    brief:
      - "***********************************************************************************************"
  defined_in_file: stlab/forest.hpp
  overloads:
    "template <class R>\nauto reverse_fullorder_range(R &)":
      arguments:
        - description: __OPTIONAL__
          name: x
          type: R &
      description: Mutable variant
      inline:
        description:
          - "***********************************************************************************************"
      return: __OPTIONAL__
      signature_with_names: "template <class R>\nauto reverse_fullorder_range(R & x)"
    "template <class R>\nauto reverse_fullorder_range(const R &)":
      arguments:
        - description: __OPTIONAL__
          name: x
          type: const R &
      description: Const variant
      return: __OPTIONAL__
      signature_with_names: "template <class R>\nauto reverse_fullorder_range(const R & x)"
  namespace:
    - stlab
---
