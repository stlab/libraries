---
layout: method
title: identity
hyde:
  owner: sean-parent
  brief: __OPTIONAL__
  tags:
    - method
  defined_in_file: stlab/concurrency/channel.hpp
  is_ctor: true
  overloads:
    identity():
      annotation:
        - implicit
      description: __OPTIONAL__
      signature_with_names: identity()
    identity(const stlab::identity &):
      annotation:
        - implicit
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: const stlab::identity &
          unnamed: true
      description: __OPTIONAL__
      signature_with_names: identity(const stlab::identity &)
    identity(stlab::identity &&):
      annotation:
        - implicit
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: stlab::identity &&
          unnamed: true
      description: __OPTIONAL__
      signature_with_names: identity(stlab::identity &&)
---
