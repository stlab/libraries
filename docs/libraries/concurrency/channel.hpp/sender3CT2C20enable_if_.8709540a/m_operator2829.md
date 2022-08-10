---
layout: method
title: operator()
owner: FelixPetriconi
brief: Send a new value into the channel.
example: call_operator_example.cpp
tags:
  - method
defined_in_file: concurrency/channel.hpp
overloads:
  "template <typename... A>\nvoid operator()(A &&...) const":
    arguments:
      - description: The input values of the channel that is defined by this sender
        name: args
        type: A &&...
    description: Sends a new value into the channel. This function never blocks, even if the buffer of the associated reveiver has reached its limits, defined by a `buffer_size`. So even if a limit is given, it will be ignored on an explicit send() and grow the queue however large is required.
    return: __OPTIONAL__
    signature_with_names: "template <typename... A>\nvoid operator()(A &&... args) const"
---
