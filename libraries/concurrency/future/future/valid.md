---
layout: method
title: valid
tags: [library]
pure-name: valid
defined-in-header: stlab/future.hpp 
declaration: valid()
description: Returns `true` if the future is valid.
entities:
  - kind: methods
    list:
      - name: stlab::future::valid
        pure-name: valid
        defined-in-header: stlab/future.hpp 
        declaration: bool valid() const
        description: Returns `true` if the future is connected with a valid task. A default constructed object or a canceled one returns `false`.
  - kind: parameters
  - kind: result
    description: returns `true` if the future has an associated function object, otherwise `false`; after a reset, it returns `false`
  - kind: example
    code: NoCode
---
