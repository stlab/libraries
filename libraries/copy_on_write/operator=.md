---
layout: method
title: stlab::copy_on_write::operator=
tags: [library]
scope: copy_on_write
pure-name: operator=
defined-in-header: stlab/copy_on_write.hpp
brief: Assigns a copy_on_write
description: |
    As with copy construction, assignment is a non-throwing operation which releases the old value and increments the reference count of the item being assigned to.
entities:
  - kind: methods
    list:
      - name: stlab::copy_on_write::operator=
        pure-name: operator=
        declaration: |
            auto operator=(const copy_on_write& x) noexcept -> copy_on_write&
        description: Copy assignment.
      - name: stlab::copy_on_write::operator=
        pure-name: operator=
        declaration: |
            auto operator=(copy_on_write&& x) noexcept -> copy_on_write&
        description: Move assignment.
      - name: stlab::copy_on_write::operator=
        pure-name: operator=
        declaration: |
            template <class U>
            auto operator=(U&& x) -> disable_copy_assign<U>
        description: |
            Forwarded `element_type` assignment
  - kind: result
    description: |
        `*this`
---
