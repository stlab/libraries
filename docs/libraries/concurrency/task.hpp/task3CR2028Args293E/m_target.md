---
layout: method
title: target
owner: sean-parent
brief: Get the underlying function
tags:
  - method
defined_in_file: concurrency/task.hpp
overloads:
  "template <class T>\nT * target()":
    description: Mutable variant
    return: __OPTIONAL__
    signature_with_names: "template <class T>\nT * target()"
  "template <class T>\nconst T * target() const":
    description: Const variant
    return: A pointer to the underlying function, if `target_type() == typeid(T)`. Otherwise, `nullptr`.
    signature_with_names: "template <class T>\nconst T * target() const"
---
