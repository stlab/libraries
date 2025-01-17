---
layout: method
title: exception
hyde:
  owner: sean-parent
  brief: __INLINED__
  tags:
    - method
  inline:
    brief:
      - "Precondition: is_ready()"
  defined_in_file: stlab/concurrency/future.hpp
  overloads:
    auto exception() const -> std::exception_ptr &:
      description: __INLINED__
      inline:
        description:
          - "Precondition: is_ready()"
      return: __OPTIONAL__
      signature_with_names: auto exception() const -> std::exception_ptr &
---
