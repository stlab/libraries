---
layout: method
title: make
hyde:
  owner: sean-parent
  brief: __MISSING__
  tags:
    - method
  defined_in_file: stlab/concurrency/future.hpp
  overloads:
    "template <class E, class F, class... Ts>\nstatic auto make(E &&, F &&, future<Ts>...)":
      arguments:
        - description: __OPTIONAL__
          name: executor
          type: E &&
        - description: __OPTIONAL__
          name: f
          type: F &&
        - description: __OPTIONAL__
          name: args
          type: future<Ts>...
      description: __OPTIONAL__
      return: __OPTIONAL__
      signature_with_names: "template <class E, class F, class... Ts>\nstatic auto make(E && executor, F && f, future<Ts>... args)"
---
