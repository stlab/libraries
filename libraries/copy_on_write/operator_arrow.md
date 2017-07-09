---
layout: method
title: stlab::copy_on_write::operator->
tags: [library]
scope: copy_on_write
pure-name: operator->
defined-in-header: stlab/copy_on_write.hpp
brief: Obtain a const reference to the underlying object.
description: |
    This is provided because you cannot override `operator.()`. It allows `copy_on_write` to be used with common transformation techniques, such as boost's `indirect_iterator` and `transform_iterator`.

    It does not imply "pointer" semantics.
entities:
  - kind: methods
    list:
      - name: stlab::copy_on_write::operator->
        pure-name: operator->
        declaration: |
            auto operator->() const noexcept
        description: Const accessor
  - kind: result
    description: A const reference to the underlying object
---
