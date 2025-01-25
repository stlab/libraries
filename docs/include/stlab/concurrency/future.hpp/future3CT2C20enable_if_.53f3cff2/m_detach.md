---
layout: method
title: detach
hyde:
  owner: sean-parent
  brief: __MISSING__
  tags:
    - method
  defined_in_file: stlab/concurrency/future.hpp
  overloads:
    "template <class F>\nvoid detach(F &&) &&":
      arguments:
        - description: __OPTIONAL__
          name: f
          type: F &&
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: "template <class F>\nvoid detach(F && f) &&"
    void detach() const:
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: void detach() const
---
