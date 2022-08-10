---
layout: method
title: depth_fullorder_iterator<I>
owner: fosterbrereton
brief: Constructor
tags:
  - method
defined_in_file: forest.hpp
is_ctor: true
overloads:
  depth_fullorder_iterator<I>(stlab::depth_fullorder_iterator::difference_type):
    arguments:
      - description: __OPTIONAL__
        name: d
        type: stlab::depth_fullorder_iterator::difference_type
    description: Starting depth ctor
    return: __OPTIONAL__
    signature_with_names: depth_fullorder_iterator<I>(stlab::depth_fullorder_iterator::difference_type d)
  explicit depth_fullorder_iterator<I>(I, stlab::depth_fullorder_iterator::difference_type):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: I
      - description: __OPTIONAL__
        name: d
        type: stlab::depth_fullorder_iterator::difference_type
    description: Iterator ctor
    return: __OPTIONAL__
    signature_with_names: explicit depth_fullorder_iterator<I>(I x, stlab::depth_fullorder_iterator::difference_type d)
  "template <class U>\ndepth_fullorder_iterator<I>(const depth_fullorder_iterator<U> &)":
    arguments:
      - description: __OPTIONAL__
        name: x
        type: const depth_fullorder_iterator<U> &
    description: Rebase ctor
    return: __OPTIONAL__
    signature_with_names: "template <class U>\ndepth_fullorder_iterator<I>(const depth_fullorder_iterator<U> & x)"
---
