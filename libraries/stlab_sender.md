---
layout: class
title: stlab::sender
entities:
  - kind: class
    name: stlab::sender
    pure-name: sender
    defined-in-header: stlab/channel.hpp
    declaration: |
        template <typename T> class future;
    description: This class implements the sending part of a CSP channel
    member-types:
      - type: 'value_type'
        definition: '`T`'
    member-functions:
      - name: close
        description: Closes this side of the channel
      - name: operator()
        description: Sends a value into the channel
---
