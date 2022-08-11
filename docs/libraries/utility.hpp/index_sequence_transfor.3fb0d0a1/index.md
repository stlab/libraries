---
layout: class
title: index_sequence_transform<Seq, F, Index, Count>
owner: __MISSING__
brief: __MISSING__
tags:
  - class
defined_in_file: utility.hpp
declaration: "template <class Seq, template <std::size_t> class F, std::size_t Index, std::size_t Count>\nstruct stlab::index_sequence_transform;"
ctor: unspecified
dtor: unspecified
typedefs:
  type:
    definition: index_sequence_cat_t<index_sequence_transform_t<Seq, F, Index, Count / 2>, index_sequence_transform_t<Seq, F, Index + Count / 2, Count - Count / 2>>
    description: __MISSING__
namespace:
  - stlab
  - v1
---
