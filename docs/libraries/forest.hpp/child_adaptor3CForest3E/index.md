---
layout: class
title: child_adaptor<Forest>
owner: fosterbrereton
brief: Adaptor to more easily manipulate a node's immediate children
tags:
  - class
defined_in_file: forest.hpp
declaration: "template <class Forest>\nclass stlab::child_adaptor;"
dtor: unspecified
typedefs:
  const_reference:
    definition: typename Forest::const_reference
    description: const reference type
  difference_type:
    definition: typename Forest::difference_type
    description: difference type
  forest_type:
    definition: Forest
    description: Forest type
  iterator:
    definition: typename Forest::child_iterator
    description: child iterator type
  iterator_type:
    definition: typename Forest::iterator
    description: iterator type
  reference:
    definition: typename Forest::reference
    description: reference type
  value_type:
    definition: typename Forest::value_type
    description: value type
namespace:
  - stlab
---
