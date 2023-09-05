---
layout: method
title: insert
hyde:
  owner: fosterbrereton
  brief: Insert a node
  tags:
    - method
  inline:
    brief:
      - "***********************************************************************************************"
  defined_in_file: forest.hpp
  overloads:
    stlab::forest::iterator insert(const stlab::forest::iterator &, T):
      arguments:
        - description: __OPTIONAL__
          name: position
          type: const stlab::forest::iterator &
        - description: __OPTIONAL__
          name: x
          type: T
      description: Single value variant
      return: __OPTIONAL__
      signature_with_names: stlab::forest::iterator insert(const stlab::forest::iterator & position, T x)
    stlab::forest::iterator insert(stlab::forest::iterator, stlab::forest::const_child_iterator, stlab::forest::const_child_iterator):
      arguments:
        - description: __OPTIONAL__
          name: position
          type: stlab::forest::iterator
        - description: __OPTIONAL__
          name: first
          type: stlab::forest::const_child_iterator
        - description: __OPTIONAL__
          name: last
          type: stlab::forest::const_child_iterator
      description: Multi-node variant. Children will be spliced from their current position into children of `position`.
      inline:
        description:
          - "***********************************************************************************************"
      return: __OPTIONAL__
      signature_with_names: stlab::forest::iterator insert(stlab::forest::iterator position, stlab::forest::const_child_iterator first, stlab::forest::const_child_iterator last)
---
