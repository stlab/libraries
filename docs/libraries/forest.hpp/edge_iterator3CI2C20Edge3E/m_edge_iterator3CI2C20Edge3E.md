---
layout: method
title: edge_iterator<I, Edge>
owner: fosterbrereton
brief: Constructor
tags:
  - method
defined_in_file: forest.hpp
is_ctor: true
overloads:
  edge_iterator<I, Edge>():
    annotation:
      - default
    description: Default ctor
    return: __OPTIONAL__
    signature_with_names: edge_iterator<I, Edge>()
  explicit edge_iterator<I, Edge>(I):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: I
    description: Iterator ctor
    return: __OPTIONAL__
    signature_with_names: explicit edge_iterator<I, Edge>(I x)
  "template <class U>\nedge_iterator<I, Edge>(const edge_iterator<U, Edge> &)":
    arguments:
      - description: __OPTIONAL__
        name: u
        type: const edge_iterator<U, Edge> &
    description: Rebase ctor
    return: __OPTIONAL__
    signature_with_names: "template <class U>\nedge_iterator<I, Edge>(const edge_iterator<U, Edge> & u)"
---
