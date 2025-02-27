---
layout: function
title: stlab_at_pre_exit
hyde:
  owner: __MISSING__
  brief: __INLINED__
  tags:
    - function
  inline:
    brief:
      - An `extern "C"` vector for `at_pre-exit()` to make it simpler to export the function from a shared library.
  defined_in_file: stlab/pre_exit.hpp
  overloads:
    void stlab_at_pre_exit(stlab::pre_exit_handler):
      arguments:
        - description: __OPTIONAL__
          name: f
          type: stlab::pre_exit_handler
      description: __INLINED__
      inline:
        description:
          - An `extern "C"` vector for `at_pre-exit()` to make it simpler to export the function from a shared library.
      return: __OPTIONAL__
      signature_with_names: void stlab_at_pre_exit(stlab::pre_exit_handler f)
  namespace:
    - stlab
    - v2
---
