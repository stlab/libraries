---
layout: method
title: make
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: concurrency/future.hpp
overloads:
  "template <typename E, typename F, typename... Ts>\nstatic auto make(E, F, future<T>, future<Ts>...)":
    arguments:
      - description: __OPTIONAL__
        name: executor
        type: E
      - description: __OPTIONAL__
        name: f
        type: F
      - description: __OPTIONAL__
        name: arg
        type: future<T>
      - description: __OPTIONAL__
        name: args
        type: future<Ts>...
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <typename E, typename F, typename... Ts>\nstatic auto make(E executor, F f, future<T> arg, future<Ts>... args)"
---
