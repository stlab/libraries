---
layout: method
title: stlab::buffer_size
tags: [library]
pure-name: buffer_size
defined-in-header: stlab/concurrency/channel.hpp 
declaration: buffer_size()
description: Constructs a new buffer_size object
entities:
  - kind: methods
    list:
      - name: stlab::buffer_size::buffer_size
        pure-name: buffer_size
        defined-in-header: stlab/concurrency/channel.hpp 
        declaration: buffer_size(std::size_t sz)
        description: Constructs a new buffer_size object and initializes it with the value sz.
  - kind: parameters
    list:
      - name: sz
        description: Specifies the size of the buffer
example: buffer_size_example.cpp
---
