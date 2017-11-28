---
layout: method
title: stlab::copy_on_write::unique
tags: [library]
scope: copy_on_write
pure-name: unique
defined-in-header: stlab/copy_on_write.hpp
brief: Returns if the object instance reference count is one
entities:
  - kind: methods
    list:
      - name: stlab::copy_on_write::unique
        pure-name: unique
        declaration: |
            bool unique() const noexcept
        description: Returns if the object instance reference count is one. This is useful to determine if writing will cause a copy.
      - name: stlab::copy_on_write::unique_instance
        pure-name: unique_instance
        declaration: |
            bool unique_instance() const noexcept
        description: Forwarding routine for `unique`.
        annotation: |
            [[deprecated]]
  - kind: result
    description: |
        `true` iff the object instance reference count is one. `false` otherwise.
---
