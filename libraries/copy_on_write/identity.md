---
layout: method
title: stlab::copy_on_write::identity
tags: [library]
scope: copy_on_write
pure-name: identity
defined-in-header: stlab/copy_on_write.hpp
git-link: https://github.com/stlab/libraries/blob/develop/stlab/copy_on_write.hpp
brief: Used to see if two copy_on_write items refer to the same instance.
entities:
  - kind: methods
    list:
      - name: stlab::copy_on_write::identity
        pure-name: identity
        declaration: |
            bool identity(const copy_on_write& x) const noexcept
        description: Used to see if two copy_on_write items refer to the same instance.
  - kind: result
    description: |
        `true` iff the underlying object instance is shared by both objects. `false` otherwise.
---
