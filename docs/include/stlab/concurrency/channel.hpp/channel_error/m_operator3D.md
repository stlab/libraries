---
layout: method
title: operator=
hyde:
  owner: sean-parent
  brief: __OPTIONAL__
  tags:
    - method
  defined_in_file: stlab/concurrency/channel.hpp
  overloads:
    stlab::channel_error & operator=(const stlab::channel_error &):
      annotation:
        - implicit
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: const stlab::channel_error &
          unnamed: true
      description: __OPTIONAL__
      return: __OPTIONAL__
      signature_with_names: stlab::channel_error & operator=(const stlab::channel_error &)
    stlab::channel_error & operator=(stlab::channel_error &&):
      annotation:
        - implicit
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: stlab::channel_error &&
          unnamed: true
      description: __OPTIONAL__
      return: __OPTIONAL__
      signature_with_names: stlab::channel_error & operator=(stlab::channel_error &&)
---
