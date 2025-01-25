---
layout: function
title: operator&
hyde:
  owner: sean-parent
  brief: __MISSING__
  tags:
    - function
  defined_in_file: stlab/concurrency/executor_base.hpp
  overloads:
    "template <typename F>\nauto operator&(F &&, stlab::executor) -> executor_task_pair<F>":
      arguments:
        - description: __OPTIONAL__
          name: f
          type: F &&
        - description: __OPTIONAL__
          name: e
          type: stlab::executor
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: "template <typename F>\nauto operator&(F && f, stlab::executor e) -> executor_task_pair<F>"
    "template <typename F>\nauto operator&(stlab::executor, F &&) -> executor_task_pair<F>":
      arguments:
        - description: __OPTIONAL__
          name: e
          type: stlab::executor
        - description: __OPTIONAL__
          name: f
          type: F &&
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: "template <typename F>\nauto operator&(stlab::executor e, F && f) -> executor_task_pair<F>"
  namespace:
    - stlab
    - v3
---
