---
layout: method
title: reverse
hyde:
  owner: fosterbrereton
  brief: Reverse a range of child nodes
  tags:
    - method
  inline:
    brief:
      - "***********************************************************************************************"
  defined_in_file: stlab/forest.hpp
  overloads:
    void reverse(stlab::forest::child_iterator, stlab::forest::child_iterator):
      arguments:
        - description: __OPTIONAL__
          name: first
          type: stlab::forest::child_iterator
        - description: __OPTIONAL__
          name: last
          type: stlab::forest::child_iterator
      description: __INLINED__
      inline:
        description:
          - "***********************************************************************************************"
      return: __OPTIONAL__
      signature_with_names: void reverse(stlab::forest::child_iterator first, stlab::forest::child_iterator last)
---
