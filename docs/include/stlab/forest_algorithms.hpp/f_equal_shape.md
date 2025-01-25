---
layout: function
title: equal_shape
hyde:
  owner: sean-parent
  brief: __INLINED__
  tags:
    - function
  inline:
    brief:
      - "***********************************************************************************************"
      - "\"Congruent\" would be a nice name here, but in geometry that also implies reflection."
  defined_in_file: stlab/forest_algorithms.hpp
  overloads:
    "template <class Forest1, class Forest2>\nauto equal_shape(const Forest1 &, const Forest2 &) -> bool":
      arguments:
        - description: __OPTIONAL__
          name: x
          type: const Forest1 &
        - description: __OPTIONAL__
          name: y
          type: const Forest2 &
      description: __INLINED__
      inline:
        description:
          - "***********************************************************************************************"
          - "\"Congruent\" would be a nice name here, but in geometry that also implies reflection."
      return: __OPTIONAL__
      signature_with_names: "template <class Forest1, class Forest2>\nauto equal_shape(const Forest1 & x, const Forest2 & y) -> bool"
  namespace:
    - stlab
    - forests
---
