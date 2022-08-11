---
layout: method
title: future_error
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: concurrency/future.hpp
is_ctor: true
overloads:
  explicit future_error(stlab::future_error_codes):
    arguments:
      - description: __OPTIONAL__
        name: code
        type: stlab::future_error_codes
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: explicit future_error(stlab::future_error_codes code)
  future_error(const stlab::future_error &):
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const stlab::future_error &
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: future_error(const stlab::future_error &)
  future_error(stlab::future_error &&):
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: stlab::future_error &&
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: future_error(stlab::future_error &&)
---
