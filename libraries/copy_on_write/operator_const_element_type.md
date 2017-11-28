---
layout: method
title: stlab::copy_on_write::operator const element_type&
tags: [library]
scope: copy_on_write
pure-name: operator const element_type&
defined-in-header: stlab/copy_on_write.hpp
brief: Obtain a const reference to the underlying object.
entities:
  - kind: methods
    list:
      - name: stlab::copy_on_write::operator const element_type&
        pure-name: operator const element_type&
        declaration: |
            auto operator const element_type&() const noexcept
        description: Const accessor
  - kind: result
    description: A const reference to the underlying object
---
