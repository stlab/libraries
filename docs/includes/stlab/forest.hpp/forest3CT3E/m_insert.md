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
  defined_in_file: stlab/forest.hpp
  overloads:
    auto insert(const stlab::forest::iterator &, T) -> stlab::forest::iterator:
      arguments:
        - description: __OPTIONAL__
          name: position
          type: const stlab::forest::iterator &
        - description: __OPTIONAL__
          name: x
          type: T
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: auto insert(const stlab::forest::iterator & position, T x) -> stlab::forest::iterator
    auto insert(stlab::forest::iterator, stlab::forest::const_child_iterator, stlab::forest::const_child_iterator) -> stlab::forest::iterator:
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
      description: __INLINED__
      inline:
        description:
          - "***********************************************************************************************"
      return: __OPTIONAL__
      signature_with_names: auto insert(stlab::forest::iterator position, stlab::forest::const_child_iterator first, stlab::forest::const_child_iterator last) -> stlab::forest::iterator
---
