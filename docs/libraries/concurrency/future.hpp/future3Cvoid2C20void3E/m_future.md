---
layout: method
title: future
hyde:
  owner: __OPTIONAL__
  brief: __OPTIONAL__
  tags:
    - method
  defined_in_file: concurrency/future.hpp
  is_ctor: true
  overloads:
    future():
      annotation:
        - defaulted
      description: __OPTIONAL__
      signature_with_names: future()
    future(const stlab::future<void> &):
      annotation:
        - implicit
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: const stlab::future<void> &
          unnamed: true
      description: __OPTIONAL__
      signature_with_names: future(const stlab::future<void> &)
    future(stlab::future<void> &&):
      annotation:
        - implicit
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: stlab::future<void> &&
          unnamed: true
      description: __OPTIONAL__
      signature_with_names: future(stlab::future<void> &&)
---
