---
layout: method
title: child_iterator<I>
owner: fosterbrereton
brief: Constructor
tags:
  - method
defined_in_file: forest.hpp
is_ctor: true
overloads:
  child_iterator<I>():
    annotation:
      - default
    description: Default ctor
    return: __OPTIONAL__
    signature_with_names: child_iterator<I>()
  explicit child_iterator<I>(I):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: I
    description: Iterator ctor
    return: __OPTIONAL__
    signature_with_names: explicit child_iterator<I>(I x)
  "template <class U>\nchild_iterator<I>(const child_iterator<U> &)":
    arguments:
      - description: __OPTIONAL__
        name: u
        type: const child_iterator<U> &
    description: Rebase ctor
    return: __OPTIONAL__
    signature_with_names: "template <class U>\nchild_iterator<I>(const child_iterator<U> & u)"
---
