---
layout: method
title: stlab::copy_on_write::copy_on_write
tags: [library]
scope: copy_on_write
pure-name: copy_on_write
defined-in-header: stlab/copy_on_write.hpp
git-link: https://github.com/stlab/libraries/blob/develop/stlab/copy_on_write.hpp
brief: Constructs a copy_on_write
entities:
  - kind: methods
    list:
      - name: stlab::copy_on_write::copy_on_write
        pure-name: copy_on_write
        declaration: |
            copy_on_write()
        description: The first call to the default constructor will construct a default instance of `element_type` which will be used for subsequent calls to the default constructor. The default instance will be released at exit.
      - name: stlab::copy_on_write::copy_on_write
        pure-name: copy_on_write
        declaration: |
            template <class U>
            copy_on_write(U&& x, disable_copy<U> = nullptr)
        description: Constructs a new `copy_on_write` object with a single argument.
      - name: stlab::copy_on_write::copy_on_write
        pure-name: copy_on_write
        declaration: |
            template <class U, class V, class... Args>
            copy_on_write(U&& x, V&& y, Args&&... args)
        description: Constructs a new `copy_on_write` object with two or more arguments.
      - name: stlab::copy_on_write::copy_on_write
        pure-name: copy_on_write
        declaration: |
            copy_on_write(const copy_on_write& x) noexcept
        description: Copy construction is a non-throwing operation and simply increments the reference count on the stored object.
      - name: stlab::copy_on_write::copy_on_write
        pure-name: copy_on_write
        declaration: |
            copy_on_write(copy_on_write&& x) noexcept
        description: Move construction.
---
