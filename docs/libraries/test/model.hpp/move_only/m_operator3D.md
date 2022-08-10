---
layout: method
title: operator=
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: test/model.hpp
overloads:
  stlab::move_only & operator=(const stlab::move_only &):
    annotation:
      - delete
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const stlab::move_only &
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: stlab::move_only & operator=(const stlab::move_only &)
  stlab::move_only & operator=(stlab::move_only &&):
    annotation:
      - default
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: stlab::move_only &&
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: stlab::move_only & operator=(stlab::move_only &&)
---
