---
layout: function
title: unwrap
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: functional.hpp
overloads:
  "template <typename T>\nT & unwrap(T &)":
    arguments:
      - description: __OPTIONAL__
        name: val
        type: T &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename T>\nT & unwrap(T & val)"
  "template <typename T>\nT & unwrap(std::reference_wrapper<T> &)":
    arguments:
      - description: __OPTIONAL__
        name: val
        type: std::reference_wrapper<T> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename T>\nT & unwrap(std::reference_wrapper<T> & val)"
  "template <typename T>\nconst T & unwrap(const T &)":
    arguments:
      - description: __OPTIONAL__
        name: val
        type: const T &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename T>\nconst T & unwrap(const T & val)"
  "template <typename T>\nconst T & unwrap(const std::reference_wrapper<T> &)":
    arguments:
      - description: __OPTIONAL__
        name: val
        type: const std::reference_wrapper<T> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename T>\nconst T & unwrap(const std::reference_wrapper<T> & val)"
namespace:
  - stlab
  - v1
---
