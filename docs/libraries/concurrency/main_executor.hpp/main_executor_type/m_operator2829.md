---
layout: method
title: operator()
owner: sean-parent
brief: Executes the given function on the application's main loop.
tags:
  - method
defined_in_file: concurrency/main_executor.hpp
overloads:
  "template <typename F>\nvoid operator()(F) const":
    arguments:
      - description: The function object that shall be executed on the main loop.
        name: f
        type: F
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\nvoid operator()(F f) const"
---
