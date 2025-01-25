---
layout: method
title: filter_fullorder_iterator<I, P>
hyde:
  owner: fosterbrereton
  brief: Constructor
  tags:
    - method
  defined_in_file: stlab/forest.hpp
  is_ctor: true
  overloads:
    filter_fullorder_iterator<I, P>():
      annotation:
        - defaulted
      description: Default ctor
      signature_with_names: filter_fullorder_iterator<I, P>()
    filter_fullorder_iterator<I, P>(I, I):
      arguments:
        - description: __OPTIONAL__
          name: f
          type: I
        - description: __OPTIONAL__
          name: l
          type: I
      description: Iterator ctor
      signature_with_names: filter_fullorder_iterator<I, P>(I f, I l)
    filter_fullorder_iterator<I, P>(I, I, P):
      arguments:
        - description: __OPTIONAL__
          name: f
          type: I
        - description: __OPTIONAL__
          name: l
          type: I
        - description: __OPTIONAL__
          name: p
          type: P
      description: Iterator/predicate ctor
      signature_with_names: filter_fullorder_iterator<I, P>(I f, I l, P p)
    "template <class U>\nfilter_fullorder_iterator<I, P>(const filter_fullorder_iterator<U, P> &)":
      arguments:
        - description: __OPTIONAL__
          name: x
          type: const filter_fullorder_iterator<U, P> &
      description: Rebase ctor
      signature_with_names: "template <class U>\nfilter_fullorder_iterator<I, P>(const filter_fullorder_iterator<U, P> & x)"
---
