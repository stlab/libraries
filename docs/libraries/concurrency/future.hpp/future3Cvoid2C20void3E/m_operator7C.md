---
layout: method
title: operator|
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: concurrency/future.hpp
overloads:
  "template <typename F>\nauto operator|(F &&) &&":
    arguments:
      - description: __OPTIONAL__
        name: f
        type: F &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\nauto operator|(F && f) &&"
  "template <typename F>\nauto operator|(F &&) const &":
    arguments:
      - description: __OPTIONAL__
        name: f
        type: F &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\nauto operator|(F && f) const &"
  "template <typename F>\nauto operator|(executor_task_pair<F>) &&":
    arguments:
      - description: __OPTIONAL__
        name: etp
        type: executor_task_pair<F>
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\nauto operator|(executor_task_pair<F> etp) &&"
  "template <typename F>\nauto operator|(executor_task_pair<F>) const &":
    arguments:
      - description: __OPTIONAL__
        name: etp
        type: executor_task_pair<F>
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\nauto operator|(executor_task_pair<F> etp) const &"
---
