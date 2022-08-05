---
layout: method
title: insert_parent
owner: fosterbrereton
brief: Insert a node as a parent of child nodes
tags:
  - method
defined_in_file: forest.hpp
overloads:
  stlab::forest::iterator insert_parent(stlab::forest::child_iterator, stlab::forest::child_iterator, const T &):
    arguments:
      - description: __OPTIONAL__
        name: front
        type: stlab::forest::child_iterator
      - description: __OPTIONAL__
        name: back
        type: stlab::forest::child_iterator
      - description: __OPTIONAL__
        name: x
        type: const T &
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: stlab::forest::iterator insert_parent(stlab::forest::child_iterator front, stlab::forest::child_iterator back, const T & x)
---
