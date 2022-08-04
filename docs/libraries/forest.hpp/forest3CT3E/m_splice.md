---
layout: method
title: splice
owner: fosterbrereton
brief: Move one or more nodes from one forest to another
tags:
  - method
defined_in_file: forest.hpp
overloads:
  stlab::forest::iterator splice(stlab::forest::iterator, forest<T> &):
    arguments:
      - description: __OPTIONAL__
        name: position
        type: stlab::forest::iterator
      - description: __OPTIONAL__
        name: x
        type: forest<T> &
    description: Full-forest splicing
    return: __OPTIONAL__
    signature_with_names: stlab::forest::iterator splice(stlab::forest::iterator position, forest<T> & x)
  stlab::forest::iterator splice(stlab::forest::iterator, forest<T> &, stlab::forest::child_iterator, stlab::forest::child_iterator):
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
    description: Child range splicing. The size of the source and destination forest will be invalidated.
    return: __OPTIONAL__
    signature_with_names: stlab::forest::iterator splice(stlab::forest::iterator position, forest<T> & x, stlab::forest::child_iterator first, stlab::forest::child_iterator last)
  stlab::forest::iterator splice(stlab::forest::iterator, forest<T> &, stlab::forest::child_iterator, stlab::forest::child_iterator, stlab::forest::size_type):
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
    description: Child range splicing. If `count` is zero, the size of the source and destination forest will be invalidated.
    return: __OPTIONAL__
    signature_with_names: stlab::forest::iterator splice(stlab::forest::iterator position, forest<T> & x, stlab::forest::child_iterator first, stlab::forest::child_iterator last, stlab::forest::size_type count)
  stlab::forest::iterator splice(stlab::forest::iterator, forest<T> &, stlab::forest::iterator):
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
    description: Parent-node splice. The children of the passed iterator will be spliced into this forest.
    return: __OPTIONAL__
    signature_with_names: stlab::forest::iterator splice(stlab::forest::iterator position, forest<T> & x, stlab::forest::iterator i)
---
