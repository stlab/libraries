---
layout: method
title: stlab::sender::operator()
tags: [library]
scope: sender
pure-name: operator()
defined-in-header: stlab/concurrency/channel.hpp 
declaration: operator()
brief: Sends a new value into the channel
description: Sends a new value into the channel
example: call_operator_example.cpp
entities:
  - kind: methods
    list:
      - name: stlab::sender::operator()
        pure-name: operator()
        defined-in-header: stlab/concurrency/channel.hpp 
        declaration: |
          template <typename... A> 
          void operator()(A&&... args) const
        description: Sends a new value into the channel. This function never blocks, even if the buffer of the associated reveiver has reached its limits, defined by a `buffer_size`. So even if a limit is given, it will be ignored on an explicit send() and grow the queue however large is required.
  - kind: parameters
    list:
      - name: args
        description: The input values of the channel that is defined by this sender
  - kind: result
---

