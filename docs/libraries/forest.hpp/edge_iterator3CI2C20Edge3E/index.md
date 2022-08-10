---
layout: class
title: edge_iterator<I, Edge>
owner: fosterbrereton
brief: Iterator that only visits either leading or trailing edges
tags:
  - class
defined_in_file: forest.hpp
declaration: "template <class I, stlab::forest_edge Edge>\nstruct stlab::edge_iterator;"
dtor: unspecified
typedefs:
  difference_type:
    definition: typename std::iterator_traits<I>::difference_type
    description: difference type
  iterator_category:
    definition: typename std::iterator_traits<I>::iterator_category
    description: iterator category
  pointer:
    definition: typename std::iterator_traits<I>::pointer
    description: pointer type
  reference:
    definition: typename std::iterator_traits<I>::reference
    description: reference type
  value_type:
    definition: typename std::iterator_traits<I>::value_type
    description: value type
namespace:
  - stlab
---
