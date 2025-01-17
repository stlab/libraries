---
layout: method
title: splice
hyde:
  owner: fosterbrereton
  brief: Move one or more nodes from one forest to another
  tags:
    - method
  inline:
    brief: _multiple descriptions_
  defined_in_file: stlab/forest.hpp
  overloads:
    auto splice(stlab::forest::iterator, forest<T> &) -> stlab::forest::iterator:
      arguments:
        - description: __OPTIONAL__
          name: position
          type: stlab::forest::iterator
        - description: __OPTIONAL__
          name: x
          type: forest<T> &
      description: __INLINED__
      inline:
        description:
          - "***********************************************************************************************"
      return: __OPTIONAL__
      signature_with_names: auto splice(stlab::forest::iterator position, forest<T> & x) -> stlab::forest::iterator
    auto splice(stlab::forest::iterator, forest<T> &, stlab::forest::child_iterator, stlab::forest::child_iterator) -> stlab::forest::iterator:
      arguments:
        - description: __OPTIONAL__
          name: position
          type: stlab::forest::iterator
        - description: __OPTIONAL__
          name: x
          type: forest<T> &
        - description: __OPTIONAL__
          name: first
          type: stlab::forest::child_iterator
        - description: __OPTIONAL__
          name: last
          type: stlab::forest::child_iterator
      description: __INLINED__
      inline:
        description:
          - "***********************************************************************************************"
      return: __OPTIONAL__
      signature_with_names: auto splice(stlab::forest::iterator position, forest<T> & x, stlab::forest::child_iterator first, stlab::forest::child_iterator last) -> stlab::forest::iterator
    auto splice(stlab::forest::iterator, forest<T> &, stlab::forest::child_iterator, stlab::forest::child_iterator, stlab::forest::size_type) -> stlab::forest::iterator:
      arguments:
        - description: __OPTIONAL__
          name: position
          type: stlab::forest::iterator
        - description: __OPTIONAL__
          name: x
          type: forest<T> &
        - description: __OPTIONAL__
          name: first
          type: stlab::forest::child_iterator
        - description: __OPTIONAL__
          name: last
          type: stlab::forest::child_iterator
        - description: __OPTIONAL__
          name: count
          type: stlab::forest::size_type
      description: __INLINED__
      inline:
        description:
          - "***********************************************************************************************"
      return: __OPTIONAL__
      signature_with_names: auto splice(stlab::forest::iterator position, forest<T> & x, stlab::forest::child_iterator first, stlab::forest::child_iterator last, stlab::forest::size_type count) -> stlab::forest::iterator
    auto splice(stlab::forest::iterator, forest<T> &, stlab::forest::iterator) -> stlab::forest::iterator:
      arguments:
        - description: __OPTIONAL__
          name: position
          type: stlab::forest::iterator
        - description: __OPTIONAL__
          name: x
          type: forest<T> &
        - description: __OPTIONAL__
          name: i
          type: stlab::forest::iterator
      description: __INLINED__
      inline:
        description:
          - "***********************************************************************************************"
      return: __OPTIONAL__
      signature_with_names: auto splice(stlab::forest::iterator position, forest<T> & x, stlab::forest::iterator i) -> stlab::forest::iterator
---
