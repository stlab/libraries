---
layout: method
title: write
owner: sean-parent
brief: Obtain a non-`const` reference to the underlying value
tags:
  - method
defined-in-file: stlab/copy_on_write.hpp
overloads:
  auto write() -> stlab::copy_on_write::element_type &:
    description: Singular variant
    return: A reference to the underlying object
    signature_with_names: auto write() -> stlab::copy_on_write::element_type &
---

Obtain a reference to the value the object is referencing. This will copy the underlying value (if necessary) so changes to the value do not affect other `copy_on_write` objects.

Note that `write()` does not have the same preconditions as `operator=()`. `write()` returns a reference to the underlying object's value, thus requiring that an underlying object exist. `operator=()` on the other hand will perform an allocation if one is necessary.
