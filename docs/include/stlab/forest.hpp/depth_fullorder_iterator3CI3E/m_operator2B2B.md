---
layout: method
title: operator++
hyde:
  owner: fosterbrereton
  brief: Increment operator
  tags:
    - method
  defined_in_file: stlab/forest.hpp
  overloads:
    auto operator++() -> auto &:
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: auto operator++() -> auto &
    auto operator++(int) -> depth_fullorder_iterator<I>:
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: int
          unnamed: true
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: auto operator++(int) -> depth_fullorder_iterator<I>
---
