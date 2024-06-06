---
layout: method
title: operator=
hyde:
  owner: sean-parent
  brief: __MISSING__
  tags:
    - method
  defined_in_file: stlab/forest_algorithms.hpp
  overloads:
    constexpr auto operator=(const typename Container::value_type &) -> auto &:
      arguments:
        - description: __OPTIONAL__
          name: value
          type: const typename Container::value_type &
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: constexpr auto operator=(const typename Container::value_type & value) -> auto &
    constexpr auto operator=(typename Container::value_type &&) -> auto &:
      arguments:
        - description: __OPTIONAL__
          name: value
          type: typename Container::value_type &&
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: constexpr auto operator=(typename Container::value_type && value) -> auto &
---
