---
layout: class
title: stlab::receiver
tags: [library]
entities:
  - kind: class
    name: stlab::receiver
    pure-name: receiver
    defined-in-header: stlab/channel.hpp
    git-link: https://github.com/stlab/libraries/blob/develop/stlab/channel.hpp
    declaration: |
        template <typename T> 
        class receiver
    description: |
      This class implements the receiving part of a CSP channel. Each receiver has an attached process that gets executed when a value is send through the sender into the channel. This attached process must either be an n-ary function object - n depends on the number of attached upstream receiver - or it must of a type that implements an `await()` and `yield()` method.
      Multiple calls with this operator on the same object realize a split. That means that the result of this process is copied into all attached downstream channels.
      In case that `T` of `receiver<T>` is a move only type, repeated calls of this operator overwrite the previous attached downstream channel.
      If an exception is thrown while calling the attached process, the exception pointer is passed to the attached process, if it has a `set_error()` method, otherwise this process is closed.
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
