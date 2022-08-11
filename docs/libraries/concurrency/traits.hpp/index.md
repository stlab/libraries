---
layout: library
title: concurrency/traits.hpp
owner: __MISSING__
brief: __MISSING__
tags:
  - sourcefile
library-type: sourcefile
typedefs:
  all_true:
    definition: std::is_same<bool_pack<true, v...>, bool_pack<v..., true>>
    description: __MISSING__
  detected_t:
    definition: typename detector<nonesuch, void, Op, Args...>::type
    description: __MISSING__
  enable_if_copyable:
    definition: std::enable_if_t<smart_is_copy_constructible_v<T>>
    description: __MISSING__
  enable_if_not_copyable:
    definition: std::enable_if_t<!smart_is_copy_constructible_v<T>>
    description: __MISSING__
  is_detected:
    definition: typename detector<nonesuch, void, Op, Args...>::value_t
    description: __MISSING__
  smart_is_copy_constructible:
    definition: smart_test<std::is_copy_constructible, T>
    description: __MISSING__
  void_t:
    definition: void
    description: __MISSING__
---
