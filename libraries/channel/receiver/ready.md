---
layout: method
title: stlab::receiver
tags: [library]
full-name: stlab::receiver::ready
pure-name: ready
defined-in-header: stlab/channel.hpp 
declaration: ready()
description: Returns `true` if the receiver is ready, otherwise `false`.
entities:
  - kind: methods
    list:
      - name: stlab::receiver::ready
        pure-name: ready
        defined-in-header: stlab/channel.hpp 
        declaration: bool ready() const
        description: Returns `true` if this receiver is ready to receive values.
  - kind: result
    description: `true`, if the receiver was set to ready, otherwise `false`; the initial value is `false`.
  - kind: parameters
    code: NoCode
---
