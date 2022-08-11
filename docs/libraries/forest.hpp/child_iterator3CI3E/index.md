---
layout: class
title: child_iterator<I>
owner: fosterbrereton
brief: Iterator type for traversing the children of a forest node
tags:
  - class
defined_in_file: forest.hpp
declaration: "template <class I>\nstruct stlab::child_iterator;"
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
