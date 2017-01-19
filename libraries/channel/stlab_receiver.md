---
layout: class
title: stlab::receiver
tags: [library]
entities:
  - kind: class
    name: stlab::receiver
    pure-name: receiver
    defined-in-header: stlab/channel.hpp
    declaration: |
        template <typename T> 
        class receiver
    description: This class implements the receiving part of a CSP channel
    member-types:
      - type: value_type
        definition: T
    member-functions:
      - name: set_ready
        description: Enabled this end of the channel to receiver values
      - name: ready
        description: Returns true if this side of the channel is enabled to receive values
      - name: operator|
        link: operator_pipe
        description: Attaches the given process to the channel
  - kind: free-functions
    functions:
      - name: channel
        defined-in-header: stlab/channel.hpp
        description: Creates a sender and a receiver pair that form a channel
        pure-name: channel
      - name: join
        defined-in-header: stlab/channel.hpp
        description: Joins several channels into a single one
        pure-name: join
      - name: merge
        defined-in-header: stlab/channel.hpp
        description: Merges several channels into a single one
        pure-name: merge
      - name: zip
        defined-in-header: stlab/channel.hpp
        description: Zips several channels into a single one
        pure-name: zip
---
