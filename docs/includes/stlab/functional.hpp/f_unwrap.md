---
layout: function
title: unwrap
hyde:
  owner: sean-parent
  brief: __INLINED__
  tags:
    - function
  inline:
    brief:
      - "***********************************************************************************************"
  defined_in_file: stlab/functional.hpp
  overloads:
    "template <typename T>\nauto unwrap(T &) -> T &":
      arguments:
        - description: __OPTIONAL__
          name: val
          type: T &
      description: __INLINED__
      inline:
        description:
          - "***********************************************************************************************"
      return: __OPTIONAL__
      signature_with_names: "template <typename T>\nauto unwrap(T & val) -> T &"
    "template <typename T>\nauto unwrap(const T &) -> const T &":
      arguments:
        - description: __OPTIONAL__
          name: val
          type: const T &
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: "template <typename T>\nauto unwrap(const T & val) -> const T &"
    "template <typename T>\nauto unwrap(const std::reference_wrapper<T> &) -> const T &":
      arguments:
        - description: __OPTIONAL__
          name: val
          type: const std::reference_wrapper<T> &
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: "template <typename T>\nauto unwrap(const std::reference_wrapper<T> & val) -> const T &"
    "template <typename T>\nauto unwrap(std::reference_wrapper<T> &) -> T &":
      arguments:
        - description: __OPTIONAL__
          name: val
          type: std::reference_wrapper<T> &
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: "template <typename T>\nauto unwrap(std::reference_wrapper<T> & val) -> T &"
  namespace:
    - stlab
    - v3
---
