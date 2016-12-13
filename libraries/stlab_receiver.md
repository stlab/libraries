---
layout: class
title: stlab::receiver
entities:
  - kind: class
    name: stlab::receiver
    pure-name: receiver
    defined-in-header: stlab/channel.hpp
    declaration: |
        template <typename T> class receiver;
    description: This class implements the receiving part of a CSP channel
    member-types:
      - type: 'value_type'
        definition: '`T`'
    member-functions:
      - name: set_ready
        description: Enabled this end of the channel to receiver values
      - name: ready
        description: Returns true if this side of the channel is enabled to receive values
      - name: operator|
        description: Pipes the result of this channel to the next instance
      - name: detach
        description: Detaches the current future
      - name: cancel_try
        description: Tries to cancel the future
      - name: get_try
        description: Tries to access the futures result
      - name: error
        description: Returns a possibly stores exception
---
