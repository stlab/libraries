---
layout: method
title: move_only
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: test/model.hpp
is_ctor: true
overloads:
  move_only():
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: move_only()
  move_only(const stlab::move_only &):
    annotation:
      - delete
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const stlab::move_only &
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: move_only(const stlab::move_only &)
  move_only(int):
    arguments:
      - description: __OPTIONAL__
        name: member
        type: int
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: move_only(int member)
  move_only(stlab::move_only &&):
    annotation:
      - default
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: stlab::move_only &&
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: move_only(stlab::move_only &&)
---
