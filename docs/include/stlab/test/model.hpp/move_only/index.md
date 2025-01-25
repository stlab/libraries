---
layout: class
title: move_only
hyde:
  owner: sean-parent
  brief: A class used to illustrate and test move-only semantics
  tags:
    - class
  inline:
    description:
      - "***********************************************************************************************"
  defined_in_file: stlab/test/model.hpp
  declaration: "\nclass stlab::move_only;"
  namespace:
    - stlab
    - v3
---

A `move_only` instance is limited to move-only assignment and construction. It contains an integer that can be used for identification and is read with the `member()` function.
