---
layout: class
title: stlab::sender
tags: [library]
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
  - kind: free-functions
    functions:
      - name: stlab::join
        defined-in-header: stlab/channel.hpp
        description: Joins several channels into a single one
        pure-name: join
      - name: stlab::merge
        defined-in-header: stlab/channel.hpp
        description: Merges several channels into a single one
        pure-name: merge
      - name: stlab::zip
        defined-in-header: stlab/channel.hpp
        description: Zips several channels into a single one
        pure-name: zip
---
