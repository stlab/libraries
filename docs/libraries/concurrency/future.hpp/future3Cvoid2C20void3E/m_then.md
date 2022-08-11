---
layout: method
title: then
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: concurrency/future.hpp
overloads:
  "template <typename E, typename F>\nauto then(E &&, F &&) &&":
    arguments:
      - description: __OPTIONAL__
        name: executor
        type: E &&
      - description: __OPTIONAL__
        name: f
        type: F &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename E, typename F>\nauto then(E && executor, F && f) &&"
  "template <typename E, typename F>\nauto then(E &&, F &&) const &":
    arguments:
      - description: __OPTIONAL__
        name: executor
        type: E &&
      - description: __OPTIONAL__
        name: f
        type: F &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename E, typename F>\nauto then(E && executor, F && f) const &"
  "template <typename F>\nauto then(F &&) &&":
    arguments:
      - description: __OPTIONAL__
        name: f
        type: F &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\nauto then(F && f) &&"
  "template <typename F>\nauto then(F &&) const &":
    arguments:
      - description: __OPTIONAL__
        name: f
        type: F &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\nauto then(F && f) const &"
---
