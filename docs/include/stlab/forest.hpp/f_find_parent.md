---
layout: function
title: find_parent
hyde:
  owner: fosterbrereton
  brief: Get the parent node of a child node
  tags:
    - function
  inline:
    brief:
      - "***********************************************************************************************"
  defined_in_file: stlab/forest.hpp
  overloads:
    "template <class I>\nauto find_parent(I) -> I":
      arguments:
        - description: __OPTIONAL__
          name: i
          type: I
      description: __INLINED__
      inline:
        description:
          - "***********************************************************************************************"
      return: __OPTIONAL__
      signature_with_names: "template <class I>\nauto find_parent(I i) -> I"
  namespace:
    - stlab
---

The time complexity for this operation is $O(N)$, where $N$ is the number of child nodes of the parent.
