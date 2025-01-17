---
layout: function
title: invoke_remove_monostate_arguments
hyde:
  owner: sean-parent
  brief: __MISSING__
  tags:
    - function
  defined_in_file: stlab/concurrency/future.hpp
  overloads:
    "template <class F, class... Args>\nauto invoke_remove_monostate_arguments(F &&, Args &&...)":
      arguments:
        - description: __OPTIONAL__
          name: f
          type: F &&
        - description: __OPTIONAL__
          name: args
          type: Args &&...
      description: __OPTIONAL__
      return: __OPTIONAL__
      signature_with_names: "template <class F, class... Args>\nauto invoke_remove_monostate_arguments(F && f, Args &&... args)"
  namespace:
    - stlab
    - v3
---
