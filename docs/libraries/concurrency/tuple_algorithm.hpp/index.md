---
layout: library
title: concurrency/tuple_algorithm.hpp
owner: __MISSING__
brief: __MISSING__
tags:
  - sourcefile
library-type: sourcefile
typedefs:
  optional_placeholder_tuple:
    definition: std::tuple<stlab::optional<typename std::conditional<std::is_same<void, Ts>::value, detail::placeholder, Ts>::type>...>
    description: __MISSING__
  placeholder_tuple:
    definition: std::tuple<typename std::conditional<std::is_same<void, Ts>::value, detail::placeholder, Ts>::type...>
    description: __MISSING__
  tuple_cat_t:
    definition: decltype(std::tuple_cat(std::declval<Ts>()...))
    description: __MISSING__
  voidless_tuple:
    definition: tuple_cat_t<typename std::conditional<std::is_same<void, Ts>::value, std::tuple<>, std::tuple<Ts>>::type...>
    description: __MISSING__
---
