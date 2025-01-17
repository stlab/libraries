---
layout: function
title: child_end
hyde:
  owner: fosterbrereton
  brief: Gets a child iterator to the one-past-the-last child of the given parent
  tags:
    - function
  inline:
    brief:
      - "***********************************************************************************************"
  defined_in_file: stlab/forest.hpp
  overloads:
    "template <class I>\nauto child_end(const I &) -> child_iterator<I>":
      arguments:
        - description: __OPTIONAL__
          name: x
          type: const I &
      description: __INLINED__
      inline:
        description:
          - "***********************************************************************************************"
      return: __OPTIONAL__
      signature_with_names: "template <class I>\nauto child_end(const I & x) -> child_iterator<I>"
  namespace:
    - stlab
---
