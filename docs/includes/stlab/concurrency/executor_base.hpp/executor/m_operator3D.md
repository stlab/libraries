---
layout: method
title: operator=
hyde:
  owner: sean-parent
  brief: __OPTIONAL__
  tags:
    - method
  defined_in_file: stlab/concurrency/executor_base.hpp
  overloads:
    stlab::executor & operator=(const stlab::executor &):
      annotation:
        - implicit
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: const stlab::executor &
          unnamed: true
      description: __OPTIONAL__
      return: __OPTIONAL__
      signature_with_names: stlab::executor & operator=(const stlab::executor &)
    stlab::executor & operator=(stlab::executor &&):
      annotation:
        - implicit
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: stlab::executor &&
          unnamed: true
      description: __OPTIONAL__
      return: __OPTIONAL__
      signature_with_names: stlab::executor & operator=(stlab::executor &&)
---
