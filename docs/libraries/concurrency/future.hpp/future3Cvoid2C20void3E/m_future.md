---
layout: method
title: future
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: concurrency/future.hpp
is_ctor: true
overloads:
  future():
    annotation:
      - default
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: future()
  future(const stlab::future<void> &):
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const stlab::future<void> &
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: future(const stlab::future<void> &)
  future(stlab::future<void> &&):
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: stlab::future<void> &&
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: future(stlab::future<void> &&)
---
