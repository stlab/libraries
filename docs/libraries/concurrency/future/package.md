---
layout: function
title: stlab::package
brief: Create a promise/future pair
defined-in-header: stlab/concurrency/future.hpp
tags:
  - function
example: package_example.cpp
overloads:
  "template <typename Sig, typename E, typename F>\nauto package(E executor, F f)":
    description: The template function package creates a pair of a promise and a future. Calling the promise will be invoked immediately, not on the provided `executor`. The purpose of the passed `executor` is to have already an executor for an attached continuation.
      If the promise is destructed and have not been called, then the associated future contains a std::exception with the error code `stlab::broken_promise`.
    arguments:
      - description: The executor is the default executor for a possible attached continuation
        name: executor
        type: E
    arguments:
      - description: Callable object to call
        name: f
        type: F
    return: A std::pair of a promise and the associated future.
    signature_with_names: "template <typename Sig, typename E, typename F>\nauto package(E executor, F f)"
---
