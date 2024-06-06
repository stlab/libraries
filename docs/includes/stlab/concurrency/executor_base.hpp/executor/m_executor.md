---
layout: method
title: executor
hyde:
  owner: sean-parent
  brief: __OPTIONAL__
  tags:
    - method
  defined_in_file: stlab/concurrency/executor_base.hpp
  is_ctor: true
  overloads:
    executor(const stlab::executor &):
      annotation:
        - implicit
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: const stlab::executor &
          unnamed: true
      description: __OPTIONAL__
      signature_with_names: executor(const stlab::executor &)
    executor(stlab::executor &&):
      annotation:
        - implicit
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: stlab::executor &&
          unnamed: true
      description: __OPTIONAL__
      signature_with_names: executor(stlab::executor &&)
---
