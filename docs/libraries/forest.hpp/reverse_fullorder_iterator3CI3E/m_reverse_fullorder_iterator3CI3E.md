---
layout: method
title: reverse_fullorder_iterator<I>
owner: fosterbrereton
brief: Constructor
tags:
  - method
defined_in_file: forest.hpp
is_ctor: true
overloads:
  explicit reverse_fullorder_iterator<I>(I):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: I
    description: Iterator ctor
    return: __OPTIONAL__
    signature_with_names: explicit reverse_fullorder_iterator<I>(I x)
  reverse_fullorder_iterator<I>():
    description: Default ctor
    return: __OPTIONAL__
    signature_with_names: reverse_fullorder_iterator<I>()
  "template <class U>\nreverse_fullorder_iterator<I>(const reverse_fullorder_iterator<U> &)":
    arguments:
      - description: __OPTIONAL__
        name: x
        type: const reverse_fullorder_iterator<U> &
    description: Rebase ctor
    return: __OPTIONAL__
    signature_with_names: "template <class U>\nreverse_fullorder_iterator<I>(const reverse_fullorder_iterator<U> & x)"
---
