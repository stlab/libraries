---
layout: method
title: read
owner: sean-parent
brief: Obtain a const reference to the underlying object.
tags:
  - method
defined-in-file: stlab/copy_on_write.hpp
overloads:
  auto read() const -> const stlab::copy_on_write::element_type &:
    description: Singular variant
    return: A const reference to the underlying object
    signature_with_names: auto read() const -> const stlab::copy_on_write::element_type &
---
