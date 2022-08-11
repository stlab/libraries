---
layout: class
title: move_only
owner: sean-parent
brief: A class used to illustrate and test move-only semantics
example: move_only_rvo_example.cpp
tags:
  - class
defined_in_file: test/model.hpp
declaration: "\nclass stlab::move_only;"
namespace:
  - stlab
  - v1
---

A `move_only` instance is limited to move-only assignment and construction. It contains an integer that can be used for identification and is read with the `member()` function.