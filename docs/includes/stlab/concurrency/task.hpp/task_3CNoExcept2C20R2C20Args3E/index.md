---
layout: class
title: task_<NoExcept, R, Args>
hyde:
  owner: sean-parent
  brief: __MISSING__
  tags:
    - class
  inline:
    description:
      - tasks are functions with a mutable call operator to support moving items through for single invocations.
  defined_in_file: stlab/concurrency/task.hpp
  declaration: "template <bool NoExcept, class R, class... Args>\nclass stlab::task_;"
  typedefs:
    result_type:
      definition: R
      description: __MISSING__
  fields:
    _invoke:
      description: __MISSING__
      type: const stlab::task_::invoke_t
    _vtable:
      description: __MISSING__
      type: const stlab::task_::concept_t
  namespace:
    - stlab
    - v3
---
