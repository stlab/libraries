---
layout: method
title: valid
owner: sean-parent
brief: Returns `true` if the future is valid.
tags:
  - method
defined_in_file: concurrency/future.hpp
overloads:
  bool valid() const:
    description: Returns `true` if the future is connected with a valid task. A default constructed object or a canceled one returns `false`.
    return: "`true` if the future has an associated function object, otherwise `false`; after a reset, it returns `false`"
    signature_with_names: bool valid() const
---
