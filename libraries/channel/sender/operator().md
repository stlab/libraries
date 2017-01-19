---
layout: method
title: stlab::sender
tags: [library]
full-name: stlab::sender::operator()
pure-name: operator()
defined-in-header: stlab/channel.hpp 
declaration: operator()
description: Sends a new value into the channel
entities:
  - kind: methods
    list:
      - name: stlab::sender::operator()
        pure-name: operator()
        defined-in-header: stlab/channel.hpp 
        declaration: |
          template <typename... A> 
          void operator()(A&&... args) const
        description: Sends a new value into the channel.
  - kind: parameters
    list:
      - name: args
        description: The input values of the channel that is defined by this sender
  - kind: result
    description: (none)
  - kind: example
    code: NoCode
---
