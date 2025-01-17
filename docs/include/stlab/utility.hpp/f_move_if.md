---
layout: function
title: move_if
hyde:
  owner: sean-parent
  brief: __INLINED__
  tags:
    - function
  inline:
    brief:
      - "***********************************************************************************************"
  defined_in_file: stlab/utility.hpp
  overloads:
    "template <bool P, class T>\nconstexpr auto move_if(T &&) -> detail::move_if_helper_t<P, T>":
      arguments:
        - description: __OPTIONAL__
          name: t
          type: T &&
      description: __INLINED__
      inline:
        description:
          - "***********************************************************************************************"
      return: __OPTIONAL__
      signature_with_names: "template <bool P, class T>\nconstexpr auto move_if(T && t) -> detail::move_if_helper_t<P, T>"
  namespace:
    - stlab
    - v3
---
