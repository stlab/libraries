---
layout: method
title: operator|
hyde:
  owner: sean-parent
  brief: __MISSING__
  tags:
    - method
  defined_in_file: stlab/concurrency/future.hpp
  overloads:
    "template <class F>\nauto operator|(F &&) &&":
      arguments:
        - description: __OPTIONAL__
          name: f
          type: F &&
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: "template <class F>\nauto operator|(F && f) &&"
    "template <class F>\nauto operator|(executor_task_pair<F>) &&":
      arguments:
        - description: __OPTIONAL__
          name: etp
          type: executor_task_pair<F>
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: "template <class F>\nauto operator|(executor_task_pair<F> etp) &&"
---
