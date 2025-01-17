---
layout: function
title: invoke_void_to_monostate_result
hyde:
  owner: sean-parent
  brief: __INLINED__
  tags:
    - function
  inline:
    brief:
      - invoke mapping void to std::monostate
  defined_in_file: stlab/concurrency/future.hpp
  overloads:
    "template <class F, class... Args>\nauto invoke_void_to_monostate_result(F &&, Args &&...)":
      arguments:
        - description: __OPTIONAL__
          name: f
          type: F &&
        - description: __OPTIONAL__
          name: args
          type: Args &&...
      description: __INLINED__
      inline:
        description:
          - invoke mapping void to std::monostate
      return: __OPTIONAL__
      signature_with_names: "template <class F, class... Args>\nauto invoke_void_to_monostate_result(F && f, Args &&... args)"
  namespace:
    - stlab
    - v3
---
