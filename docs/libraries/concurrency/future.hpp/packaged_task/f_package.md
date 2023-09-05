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
  defined_in_file: concurrency/future.hpp
  overloads:
    "template <typename Sig, typename E, typename F>\nauto package(E, F &&) -> std::pair<detail::packaged_task_from_signature_t<Sig>, future<detail::result_of_t_<Sig>>>":
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
      signature_with_names: "template <typename Sig, typename E, typename F>\nauto package(E executor, F && f) -> std::pair<detail::packaged_task_from_signature_t<Sig>, future<detail::result_of_t_<Sig>>>"
    "template <typename Signature, typename E, typename F>\nauto package(E, F &&) -> std::pair<detail::packaged_task_from_signature_t<Signature>, future<detail::result_of_t_<Signature>>>":
      arguments:
        - description: The executor is the default executor for a possible attached continuation
          name: unnamed-0
          type: E
          unnamed: true
        - description: Callable object to call
          name: unnamed-1
          type: F &&
          unnamed: true
      description: The template function package creates a pair of a promise and a future. Calling the promise will be invoked immediately, not on the provided `executor`. The purpose of the passed `executor` is to have already an executor for an attached continuation. If the promise is destructed and have not been called, then the associated future contains a std::exception with the error code `stlab::broken_promise`.
      inline:
        description:
          - "***********************************************************************************************"
      return: A std::pair of a promise and the associated future.
      signature_with_names: "template <typename Signature, typename E, typename F>\nauto package(E, F &&) -> std::pair<detail::packaged_task_from_signature_t<Signature>, future<detail::result_of_t_<Signature>>>"
  namespace:
    - stlab
    - v1
---
