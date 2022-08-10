---
layout: function
title: package_with_broken_promise
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: concurrency/future.hpp
overloads:
  "template <typename Sig, typename E, typename F>\nauto package_with_broken_promise(E, F &&) -> std::pair<detail::packaged_task_from_signature_t<Sig>, future<detail::result_of_t_<Sig>>>":
    arguments:
      - description: __OPTIONAL__
        name: executor
        type: E
      - description: __OPTIONAL__
        name: f
        type: F &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename Sig, typename E, typename F>\nauto package_with_broken_promise(E executor, F && f) -> std::pair<detail::packaged_task_from_signature_t<Sig>, future<detail::result_of_t_<Sig>>>"
  "template <typename Signature, typename E, typename F>\nauto package_with_broken_promise(E, F &&) -> std::pair<detail::packaged_task_from_signature_t<Signature>, future<detail::result_of_t_<Signature>>>":
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: E
        unnamed: true
      - description: __OPTIONAL__
        name: unnamed-1
        type: F &&
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename Signature, typename E, typename F>\nauto package_with_broken_promise(E, F &&) -> std::pair<detail::packaged_task_from_signature_t<Signature>, future<detail::result_of_t_<Signature>>>"
namespace:
  - stlab
  - v1
---
