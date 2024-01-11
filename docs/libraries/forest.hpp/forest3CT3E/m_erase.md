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
  defined_in_file: forest.hpp
  overloads:
    stlab::forest::iterator erase(const stlab::forest::iterator &):
      arguments:
        - description: __OPTIONAL__
          name: position
          type: const stlab::forest::iterator &
      description: Single node erasing
      inline:
        description:
          - "***********************************************************************************************"
      return: __OPTIONAL__
      signature_with_names: stlab::forest::iterator erase(const stlab::forest::iterator & position)
    stlab::forest::iterator erase(const stlab::forest::iterator &, const stlab::forest::iterator &):
      arguments:
        - description: __OPTIONAL__
          name: first
          type: const stlab::forest::iterator &
        - description: __OPTIONAL__
          name: last
          type: const stlab::forest::iterator &
      description: Multi-node erasing
      inline:
        description:
          - "***********************************************************************************************"
      return: __OPTIONAL__
      signature_with_names: stlab::forest::iterator erase(const stlab::forest::iterator & first, const stlab::forest::iterator & last)
---

The single-node variant lets you remove a node from anywhere within the forest. Removing a parent node will cause its children to become peers of the former parent's siblings.
