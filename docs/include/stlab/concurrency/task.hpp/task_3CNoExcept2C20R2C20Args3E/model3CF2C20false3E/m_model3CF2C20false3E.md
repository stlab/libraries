---
layout: method
title: model<F, false>
hyde:
  owner: sean-parent
  brief: __OPTIONAL__
  tags:
    - method
  defined_in_file: stlab/concurrency/task.hpp
  is_ctor: true
  overloads:
    model<F, false>(model<F, false> &&):
      annotation:
        - defaulted
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: model<F, false> &&
          unnamed: true
      description: __OPTIONAL__
      signature_with_names: model<F, false>(model<F, false> &&)
    "template <class G>\nmodel<F, false>(G &&)":
      arguments:
        - description: __OPTIONAL__
          name: f
          type: G &&
      description: __MISSING__
      signature_with_names: "template <class G>\nmodel<F, false>(G && f)"
---
