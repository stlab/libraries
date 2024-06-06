---
layout: method
title: model<F, true>
hyde:
  owner: sean-parent
  brief: __OPTIONAL__
  tags:
    - method
  defined_in_file: stlab/concurrency/task.hpp
  is_ctor: true
  overloads:
    model<F, true>(model<F, true> &&):
      annotation:
        - deleted
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: model<F, true> &&
          unnamed: true
      description: __OPTIONAL__
      signature_with_names: model<F, true>(model<F, true> &&)
    "template <class G>\nmodel<F, true>(G &&)":
      arguments:
        - description: __OPTIONAL__
          name: f
          type: G &&
      description: __MISSING__
      signature_with_names: "template <class G>\nmodel<F, true>(G && f)"
---
