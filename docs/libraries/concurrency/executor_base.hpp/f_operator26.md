---
layout: function
title: operator&
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: concurrency/executor_base.hpp
overloads:
  "template <typename F>\nexecutor_task_pair<F> operator&(F &&, stlab::executor)":
    arguments:
      - description: __OPTIONAL__
        name: f
        type: F &&
      - description: __OPTIONAL__
        name: e
        type: stlab::executor
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\nexecutor_task_pair<F> operator&(F && f, stlab::executor e)"
  "template <typename F>\nexecutor_task_pair<F> operator&(stlab::executor, F &&)":
    arguments:
      - description: __OPTIONAL__
        name: e
        type: stlab::executor
      - description: __OPTIONAL__
        name: f
        type: F &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\nexecutor_task_pair<F> operator&(stlab::executor e, F && f)"
namespace:
  - stlab
  - v1
---
