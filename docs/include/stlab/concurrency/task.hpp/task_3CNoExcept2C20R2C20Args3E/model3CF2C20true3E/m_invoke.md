---
layout: method
title: invoke
hyde:
  owner: sean-parent
  brief: __INLINED__
  tags:
    - method
  inline:
    brief:
      - NOLINTNEXTLINE(performance-unnecessary-value-param)
  defined_in_file: stlab/concurrency/task.hpp
  overloads:
    static auto invoke(void *, Args...) -> R:
      arguments:
        - description: __OPTIONAL__
          name: self
          type: void *
        - description: __OPTIONAL__
          name: args
          type: Args...
      description: __INLINED__
      inline:
        description:
          - NOLINTNEXTLINE(performance-unnecessary-value-param)
      return: __OPTIONAL__
      signature_with_names: static auto invoke(void * self, Args... args) -> R
---
