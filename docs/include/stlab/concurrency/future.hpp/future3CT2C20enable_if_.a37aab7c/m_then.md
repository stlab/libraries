---
layout: method
title: then
hyde:
  owner: sean-parent
  brief: __MISSING__
  tags:
    - method
  defined_in_file: stlab/concurrency/future.hpp
  overloads:
    "template <typename E, typename F>\nauto then(E &&, F &&) &&":
      arguments:
        - description: __OPTIONAL__
          name: executor
          type: E &&
        - description: __OPTIONAL__
          name: f
          type: F &&
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: "template <typename E, typename F>\nauto then(E && executor, F && f) &&"
    "template <typename F>\nauto then(F &&) &&":
      arguments:
        - description: __OPTIONAL__
          name: f
          type: F &&
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: "template <typename F>\nauto then(F && f) &&"
---
