---
layout: class
title: reverse_fullorder_iterator<I>
hyde:
  owner: fosterbrereton
  brief: Fullorder iterator that visits forest nodes in reverse
  tags:
    - class
  inline:
    description:
      - "***********************************************************************************************"
  defined_in_file: stlab/forest.hpp
  declaration: "template <class I>\nstruct stlab::reverse_fullorder_iterator;"
  dtor: unspecified
  typedefs:
    difference_type:
      definition: typename std::iterator_traits<I>::difference_type
      description: difference type
    iterator_category:
      definition: typename std::iterator_traits<I>::iterator_category
      description: iterator category
    iterator_type:
      definition: I
      description: iterator type
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
