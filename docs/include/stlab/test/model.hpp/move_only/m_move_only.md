---
layout: method
title: move_only
hyde:
  owner: sean-parent
  brief: __OPTIONAL__
  tags:
    - method
  defined_in_file: stlab/test/model.hpp
  is_ctor: true
  overloads:
    move_only():
      annotation:
        - defaulted
      description: __OPTIONAL__
      signature_with_names: move_only()
    move_only(const stlab::move_only &):
      annotation:
        - deleted
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: const stlab::move_only &
          unnamed: true
      description: __OPTIONAL__
      signature_with_names: move_only(const stlab::move_only &)
    move_only(int):
      arguments:
        - description: __OPTIONAL__
          name: member
          type: int
      description: __MISSING__
      signature_with_names: move_only(int member)
    move_only(stlab::move_only &&):
      annotation:
        - defaulted
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: stlab::move_only &&
          unnamed: true
      description: __OPTIONAL__
      signature_with_names: move_only(stlab::move_only &&)
---
