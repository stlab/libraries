---
layout: method
title: buffer_size
hyde:
  owner: FelixPetriconi
  brief: buffer_size constructor
  tags:
    - method
  defined_in_file: stlab/concurrency/channel.hpp
  is_ctor: true
  overloads:
    buffer_size(std::size_t):
      arguments:
        - description: Specifies the size of the buffer
          name: b
          type: std::size_t
      description: Constructs a new buffer_size object and initializes it with the value `b`.
      signature_with_names: buffer_size(std::size_t b)
---
