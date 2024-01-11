---
layout: library
title: concurrency/tuple_algorithm.hpp
hyde:
  owner: __MISSING__
  brief: __MISSING__
  tags:
    - sourcefile
  library-type: sourcefile
  typedefs:
    optional_placeholder_tuple:
      definition: std::tuple<std::optional<typename std::conditional<std::is_same<void, Ts>::value, detail::placeholder, Ts>::type>...>
      description: __INLINED__
      inline:
        description:
          - "***********************************************************************************************"
          - type-function that takes a parameter pack and returns a std::tuple <Ts ...> where all T[i] = void have been replaced with stlab::placeholder.
    placeholder_tuple:
      definition: std::tuple<typename std::conditional<std::is_same<void, Ts>::value, detail::placeholder, Ts>::type...>
      description: __INLINED__
      inline:
        description:
          - "***********************************************************************************************"
          - type-function that takes a parameter pack and returns a std::tuple <Ts ...> where all T[i] = void have been replaced with stlab::placeholder.
    tuple_cat_t:
      definition: decltype(std::tuple_cat(std::declval<Ts>()...))
      description: __INLINED__
      inline:
        description:
          - "***********************************************************************************************"
          - type-function variant of std::tuple_cat
    voidless_tuple:
      definition: tuple_cat_t<typename std::conditional<std::is_same<void, Ts>::value, std::tuple<>, std::tuple<Ts>>::type...>
      description: __INLINED__
      inline:
        description:
          - "***********************************************************************************************"
          - type-function that takes a parameter pack and returns a std::tuple <Ts ...> where all T[i] = void have been removed.
---
