---
layout: method
title: insert_parent
hyde:
  owner: fosterbrereton
  brief: Insert a node as a parent of child nodes
  tags:
    - method
  inline:
    brief:
      - "***********************************************************************************************"
  defined_in_file: stlab/forest.hpp
  overloads:
    auto insert_parent(stlab::forest::child_iterator, stlab::forest::child_iterator, const T &) -> stlab::forest::iterator:
      arguments:
        - description: __OPTIONAL__
          name: front
          type: stlab::forest::child_iterator
        - description: __OPTIONAL__
          name: back
          type: stlab::forest::child_iterator
        - description: __OPTIONAL__
          name: x
          type: const T &
      description: __INLINED__
      inline:
        description:
          - "***********************************************************************************************"
      return: __OPTIONAL__
      signature_with_names: auto insert_parent(stlab::forest::child_iterator front, stlab::forest::child_iterator back, const T & x) -> stlab::forest::iterator
---
