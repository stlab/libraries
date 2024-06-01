---
layout: method
title: operator=
hyde:
  owner: sean-parent
  brief: __OPTIONAL__
  tags:
    - method
  defined_in_file: stlab/concurrency/serial_queue.hpp
  overloads:
    stlab::serial_queue_t & operator=(const stlab::serial_queue_t &):
      annotation:
        - implicit
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: const stlab::serial_queue_t &
          unnamed: true
      description: __OPTIONAL__
      return: __OPTIONAL__
      signature_with_names: stlab::serial_queue_t & operator=(const stlab::serial_queue_t &)
    stlab::serial_queue_t & operator=(stlab::serial_queue_t &&):
      annotation:
        - implicit
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: stlab::serial_queue_t &&
          unnamed: true
      description: __OPTIONAL__
      return: __OPTIONAL__
      signature_with_names: stlab::serial_queue_t & operator=(stlab::serial_queue_t &&)
---
