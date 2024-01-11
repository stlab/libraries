---
layout: method
title: operator=
hyde:
  owner: __OPTIONAL__
  brief: __OPTIONAL__
  tags:
    - method
  defined_in_file: concurrency/future.hpp
  overloads:
    stlab::future<void> & operator=(const stlab::future<void> &):
      annotation:
        - implicit
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: const stlab::future<void> &
          unnamed: true
      description: __OPTIONAL__
      return: __OPTIONAL__
      signature_with_names: stlab::future<void> & operator=(const stlab::future<void> &)
    stlab::future<void> & operator=(stlab::future<void> &&):
      annotation:
        - implicit
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: stlab::future<void> &&
          unnamed: true
      description: __OPTIONAL__
      return: __OPTIONAL__
      signature_with_names: stlab::future<void> & operator=(stlab::future<void> &&)
---
