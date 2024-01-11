---
layout: method
title: root
hyde:
  owner: fosterbrereton
  brief: Get a fullorder iterator to the root node on the leading edge
  tags:
    - method
  inline:
    brief:
      - iterators
  defined_in_file: forest.hpp
  overloads:
    stlab::forest::const_iterator root() const:
      description: Const variant
      return: __OPTIONAL__
      signature_with_names: stlab::forest::const_iterator root() const
    stlab::forest::iterator root():
      description: Mutable variant
      inline:
        description:
          - iterators
      return: __OPTIONAL__
      signature_with_names: stlab::forest::iterator root()
---
