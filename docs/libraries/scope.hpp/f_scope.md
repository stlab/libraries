---
layout: function
title: scope
owner: sean-parent
brief: Explicitly scope the lifetime of an object
example: [ scope_example.cpp, scope_example_return.cpp ]
tags:
  - function
defined_in_file: scope.hpp
overloads:
  "template <typename T, typename F>\nauto scope(std::mutex &, F &&)":
    arguments:
      - description: __OPTIONAL__
        name: m
        type: std::mutex &
      - description: __OPTIONAL__
        name: f
        type: F &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename T, typename F>\nauto scope(std::mutex & m, F && f)"
  "template <typename T, typename... Args>\nauto scope(Args &&...)":
    arguments:
      - description: Range of parameters forwarded to the constructor of `T`
        name: |
          [ args[0], args[N-1] )
        type: Args &&...
      - description: A callable nullary function
        name: args[N-1]
    description: Scopes the lifetime of an instance of `T`. All but the last parameters are used to construct `T`, while the last parameter is assumed to be a nullary function, and is called. After the nullary function goes out of scope, `T` is destroyed.
    return: __OPTIONAL__
    signature_with_names: "template <typename T, typename... Args>\nauto scope(Args &&... args)"
namespace:
  - stlab
  - v1
---
