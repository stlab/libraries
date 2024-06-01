---
layout: function
title: package
hyde:
  owner: sean-parent
  brief: Create a promise/future pair
  tags:
    - function
  inline:
    brief: _multiple descriptions_
  defined_in_file: stlab/concurrency/future.hpp
  overloads:
    "template <typename Sig, typename E, typename F>\nauto package(E, F &&) -> std::pair<detail::packaged_task_from_signature_t<Sig>, detail::reduced_result_t<Sig>>":
      arguments:
        - description: __OPTIONAL__
          name: executor
          type: E
          unnamed: true
        - description: __OPTIONAL__
          name: f
          type: F &&
          unnamed: true
      description: __INLINED__
      inline:
        description:
          - "***********************************************************************************************"
      return: __OPTIONAL__
      signature_with_names: "template <typename Sig, typename E, typename F>\nauto package(E executor, F && f) -> std::pair<detail::packaged_task_from_signature_t<Sig>, detail::reduced_result_t<Sig>>"
  namespace:
    - stlab
    - v3
---
