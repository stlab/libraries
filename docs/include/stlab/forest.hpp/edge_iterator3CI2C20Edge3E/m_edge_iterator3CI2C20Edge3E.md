---
layout: method
title: edge_iterator<I, Edge>
hyde:
  owner: fosterbrereton
  brief: Constructor
  tags:
    - method
  defined_in_file: stlab/forest.hpp
  is_ctor: true
  overloads:
    edge_iterator<I, Edge>():
      annotation:
        - defaulted
      description: Default ctor
      signature_with_names: edge_iterator<I, Edge>()
    explicit edge_iterator<I, Edge>(const I &):
      arguments:
        - description: __OPTIONAL__
          name: x
          type: const I &
      description: __MISSING__
      signature_with_names: explicit edge_iterator<I, Edge>(const I & x)
    "template <class U>\nedge_iterator<I, Edge>(const edge_iterator<U, Edge> &)":
      arguments:
        - description: __OPTIONAL__
          name: u
          type: const edge_iterator<U, Edge> &
      description: Rebase ctor
      signature_with_names: "template <class U>\nedge_iterator<I, Edge>(const edge_iterator<U, Edge> & u)"
---
