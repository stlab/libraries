---
layout: method
title: stlab::copy_on_write::read
tags: [library]
scope: copy_on_write
pure-name: read
defined-in-header: stlab/copy_on_write.hpp
git-link: https://github.com/stlab/libraries/blob/develop/stlab/copy_on_write.hpp
brief: Obtain a const reference to the underlying object.
entities:
  - kind: methods
    list:
      - name: stlab::copy_on_write::read
        pure-name: read
        declaration: |
            auto read() const noexcept -> const element_type&
        description: Const accessor
  - kind: result
    description: A const reference to the underlying object
---
