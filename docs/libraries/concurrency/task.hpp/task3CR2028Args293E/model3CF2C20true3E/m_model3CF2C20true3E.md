---
layout: method
title: model<F, true>
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: concurrency/task.hpp
is_ctor: true
overloads:
  model<F, true>(model<F, true> &&):
    annotation:
      - delete
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: model<F, true> &&
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: model<F, true>(model<F, true> &&)
  "template <class G>\nmodel<F, true>(G &&)":
    arguments:
      - description: __OPTIONAL__
        name: f
        type: G &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class G>\nmodel<F, true>(G && f)"
---
