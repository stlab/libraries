---
layout: method
title: future_error
hyde:
  owner: sean-parent
  brief: __OPTIONAL__
  tags:
    - method
  defined_in_file: stlab/concurrency/future.hpp
  is_ctor: true
  overloads:
    explicit future_error(stlab::future_error_codes):
      arguments:
        - description: __OPTIONAL__
          name: code
          type: stlab::future_error_codes
      description: __MISSING__
      signature_with_names: explicit future_error(stlab::future_error_codes code)
    future_error(const stlab::future_error &):
      annotation:
        - implicit
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: const stlab::future_error &
          unnamed: true
      description: __OPTIONAL__
      signature_with_names: future_error(const stlab::future_error &)
    future_error(stlab::future_error &&):
      annotation:
        - implicit
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: stlab::future_error &&
          unnamed: true
      description: __OPTIONAL__
      signature_with_names: future_error(stlab::future_error &&)
---
