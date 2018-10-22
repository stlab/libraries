---
layout: method
title: read
owner: sparent
brief: Obtain a const reference to the underlying object.
tags:
  - method
defined-in-file: stlab/copy_on_write.hpp
overloads:
  auto read() const -> const stlab::copy_on_write::element_type &:
    annotation:
      - public
    description: Singular variant
    return: A const reference to the underlying object
    signature_with_names: auto read() const -> const stlab::copy_on_write::element_type &
---
