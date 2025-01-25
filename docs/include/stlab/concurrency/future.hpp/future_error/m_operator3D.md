---
layout: method
title: operator=
hyde:
  owner: sean-parent
  brief: __OPTIONAL__
  tags:
    - method
  defined_in_file: stlab/concurrency/future.hpp
  overloads:
    stlab::future_error & operator=(const stlab::future_error &):
      annotation:
        - implicit
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: const stlab::future_error &
          unnamed: true
      description: __OPTIONAL__
      return: __OPTIONAL__
      signature_with_names: stlab::future_error & operator=(const stlab::future_error &)
    stlab::future_error & operator=(stlab::future_error &&):
      annotation:
        - implicit
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: stlab::future_error &&
          unnamed: true
      description: __OPTIONAL__
      return: __OPTIONAL__
      signature_with_names: stlab::future_error & operator=(stlab::future_error &&)
---
