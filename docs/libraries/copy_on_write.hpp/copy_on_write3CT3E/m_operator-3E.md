---
layout: method
title: operator->
owner: sean-parent
brief: Obtain a `const` pointer to the underlying object
tags:
  - method
defined_in_file: copy_on_write.hpp
overloads:
  auto operator->() const -> const stlab::copy_on_write::element_type *:
    description: Singular variant
    return: A const pointer to the underlying element
    signature_with_names: auto operator->() const -> const stlab::copy_on_write::element_type *
---

This is provided because you cannot override `operator.()`. It allows `copy_on_write` to be used with common transformation techniques, such as boost's `indirect_iterator` and `transform_iterator`.

It does not imply "pointer" semantics.
