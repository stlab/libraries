---
layout: method
title: stlab::future
tags: [library]
full-name: stlab::future::valid
pure-name: valid
defined-in-header: stlab/future.hpp 
declaration: valid()
description: Returns true if the future is valid
entities:
  - kind: methods
    list:
      - name: stlab::future::valid
        pure-name: valid
        defined-in-header: stlab/future.hpp 
        declaration: bool valid() const
        description: Returns true if the future is connected with a valid task. A default constructed object or a canceled one returns false.
  - kind: example
    code: NoCode
---