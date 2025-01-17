---
layout: method
title: erase
hyde:
  owner: fosterbrereton
  brief: Remove one or more nodes
  tags:
    - method
  inline:
    brief: _multiple descriptions_
  defined_in_file: stlab/forest.hpp
  overloads:
    auto erase(const stlab::forest::iterator &) -> stlab::forest::iterator:
      arguments:
        - description: __OPTIONAL__
          name: position
          type: const stlab::forest::iterator &
      description: __INLINED__
      inline:
        description:
          - "***********************************************************************************************"
      return: __OPTIONAL__
      signature_with_names: auto erase(const stlab::forest::iterator & position) -> stlab::forest::iterator
    auto erase(const stlab::forest::iterator &, const stlab::forest::iterator &) -> stlab::forest::iterator:
      arguments:
        - description: __OPTIONAL__
          name: first
          type: const stlab::forest::iterator &
        - description: __OPTIONAL__
          name: last
          type: const stlab::forest::iterator &
      description: __INLINED__
      inline:
        description:
          - "***********************************************************************************************"
      return: __OPTIONAL__
      signature_with_names: auto erase(const stlab::forest::iterator & first, const stlab::forest::iterator & last) -> stlab::forest::iterator
---

The single-node variant lets you remove a node from anywhere within the forest. Removing a parent node will cause its children to become peers of the former parent's siblings.
