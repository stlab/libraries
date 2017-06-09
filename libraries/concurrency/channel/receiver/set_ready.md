---
layout: method
title: set_ready
tags: [library]
scope: receiver
pure-name: set_ready
defined-in-header: stlab/concurrency/channel.hpp 
declaration: set_ready
brief: Sets the receiver ready to receive values.
description: Sets the receiver ready to receive values.
example: set_ready_example.cpp
entities:
  - kind: methods
    list:
      - name: stlab::receiver::set_ready
        pure-name: set_ready
        defined-in-header: stlab/concurrency/channel.hpp 
        declaration: void set_ready()
        description: Sets this receiver ready to receiver values. That implies that no more downstream processes can be attached.
  - kind: parameters
  - kind: result
---
