---
layout: function
title: scope
hyde:
  owner: sean-parent
  brief: Explicitly scope the lifetime of an object
  tags:
    - function
  inline:
    brief: _multiple descriptions_
  defined_in_file: stlab/scope.hpp
  overloads:
    "template <typename T, typename F>\nauto scope(std::mutex &, F &&)":
      arguments:
        - description: __OPTIONAL__
          name: m
          type: std::mutex &
        - description: __OPTIONAL__
          name: f
          type: F &&
      description: __INLINED__
      inline:
        description:
          - Workaround until VS2017 bug is fixed
      return: __OPTIONAL__
      signature_with_names: "template <typename T, typename F>\nauto scope(std::mutex & m, F && f)"
    "template <typename T, typename... Args>\nauto scope(Args &&...)":
      arguments:
        - description: __OPTIONAL__
          name: args
          type: Args &&...
      description: Scopes the lifetime of an instance of `T`. All but the last parameters are used to construct `T`, while the last parameter is assumed to be a nullary function, and is called. After the nullary function goes out of scope, `T` is destroyed.
      inline:
        description:
          - "***********************************************************************************************"
      return: __OPTIONAL__
      signature_with_names: "template <typename T, typename... Args>\nauto scope(Args &&... args)"
  namespace:
    - stlab
    - v3
---
