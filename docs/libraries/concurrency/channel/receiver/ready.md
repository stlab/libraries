---
layout: method
title: ready
tags: [library]
pure-name: ready
defined-in-header: stlab/concurrency/channel.hpp 
declaration: ready()
description: Returns `true` if the receiver is ready, otherwise `false`.
entities:
  - kind: methods
    list:
      - name: stlab::receiver::ready
        pure-name: ready
        defined-in-header: stlab/concurrency/channel.hpp 
        declaration: bool ready() const
        description: Returns `true` if this receiver is ready to receive values.
  - kind: result
    description: if the receiver was set to ready, it returns `true`, otherwise `false`; the initial value is `false`.
  - kind: parameters
---
