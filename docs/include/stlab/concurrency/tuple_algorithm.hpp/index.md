---
layout: library
title: stlab/concurrency/tuple_algorithm.hpp
hyde:
  owner: sean-parent
  brief: __MISSING__
  tags:
    - sourcefile
  library-type: sourcefile
  typedefs:
    optional_placeholder_tuple:
      definition: std::tuple<std::optional<std::conditional_t<std::is_same_v<void, Ts>, placeholder, Ts>>...>
      description: __INLINED__
      inline:
        description:
          - "***********************************************************************************************"
          - type-function that takes a parameter pack and returns a std::tuple <std ::optional <Ts >...> where all T[i] == void have been replaced with stlab::placeholder.
    placeholder_tuple:
      definition: std::tuple<std::conditional_t<std::is_same_v<void, Ts>, placeholder, Ts>...>
      description: __INLINED__
      inline:
        description:
          - "***********************************************************************************************"
          - type-function that takes a parameter pack and returns a std::tuple <Ts ...> where all T[i] == void have been replaced with stlab::placeholder.
    tuple_cat_t:
      definition: decltype(std::tuple_cat(std::declval<Ts>()...))
      description: __INLINED__
      inline:
        description:
          - "***********************************************************************************************"
          - type-function variant of std::tuple_cat
    voidless_tuple:
      definition: tuple_cat_t<std::conditional_t<std::is_same_v<void, Ts>, std::tuple<>, std::tuple<Ts>>...>
      description: __INLINED__
      inline:
        description:
          - "***********************************************************************************************"
          - type-function that takes a parameter pack and returns a std::tuple <Ts ...> where all T[i] == void have been removed.
---
