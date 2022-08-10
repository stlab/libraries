---
layout: method
title: buffer_size
owner: FelixPetriconi
brief: buffer_size constructor
example: buffer_size_example.cpp
tags:
  - method
defined_in_file: concurrency/channel.hpp
is_ctor: true
overloads:
  buffer_size(std::size_t):
    arguments:
      - description: Specifies the size of the buffer
        name: b
        type: std::size_t
    description: Constructs a new buffer_size object and initializes it with the value `b`.
    return: A buffer_size object of size `b`.
    signature_with_names: buffer_size(std::size_t b)
---
