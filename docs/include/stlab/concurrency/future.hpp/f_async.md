---
layout: function
title: async
hyde:
  owner: sean-parent
  brief: Creates a future running on a given executor
  tags:
    - function
  defined_in_file: stlab/concurrency/future.hpp
  overloads:
    "template <typename E, typename F, typename... Args>\nauto async(E, F &&, Args &&...) -> detail::reduced_t<detail::result_t<std::decay_t<F>, std::decay_t<Args>...>>":
      arguments:
        - description: __OPTIONAL__
          name: executor
          type: E
        - description: __OPTIONAL__
          name: f
          type: F &&
        - description: __OPTIONAL__
          name: args
          type: Args &&...
      description: __OPTIONAL__
      return: __OPTIONAL__
      signature_with_names: "template <typename E, typename F, typename... Args>\nauto async(E executor, F && f, Args &&... args) -> detail::reduced_t<detail::result_t<std::decay_t<F>, std::decay_t<Args>...>>"
  namespace:
    - stlab
    - v3
---
