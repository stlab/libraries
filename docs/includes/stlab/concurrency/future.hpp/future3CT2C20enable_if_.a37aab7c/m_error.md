---
layout: method
title: error
hyde:
  owner: sean-parent
  brief: __MISSING__
  tags:
    - method
  defined_in_file: stlab/concurrency/future.hpp
  overloads:
    auto error() const -> std::optional<std::exception_ptr> &:
      annotation:
        - deprecated("Use exception() instead")
      description: __OPTIONAL__
      return: __OPTIONAL__
      signature_with_names: auto error() const -> std::optional<std::exception_ptr> &
---
