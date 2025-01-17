---
layout: function
title: for_each_argument
hyde:
  owner: sean-parent
  brief: __INLINED__
  tags:
    - function
  inline:
    brief:
      - Invokes `f(arg0)`, `f(arg1)`, ... `f(argN)`.
  defined_in_file: stlab/utility.hpp
  overloads:
    "template <class F, class... Args>\nvoid for_each_argument(F &&, Args &&...)":
      arguments:
        - description: Unary function that can take all arguments of type `Args`
          name: f
          type: F &&
        - description: Arguments to be passed one-by-one to `f`
          name: args
          type: Args &&...
      description: __INLINED__
      inline:
        description:
          - Invokes `f(arg0)`, `f(arg1)`, ... `f(argN)`.
      return: __OPTIONAL__
      signature_with_names: "template <class F, class... Args>\nvoid for_each_argument(F && f, Args &&... args)"
  namespace:
    - stlab
    - v3
---
