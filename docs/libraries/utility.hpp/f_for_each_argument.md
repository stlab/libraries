---
layout: function
title: for_each_argument
owner: sean-parent
brief: Execute a function for each passed argument
example: [ for_each_argument_example.cpp ]
tags:
  - function
defined_in_file: utility.hpp
overloads:
  "template <class F, class... Args>\nvoid for_each_argument(F &&, Args &&...)":
    arguments:
      - description: Unary function that can take all arguments of type `Args`
        name: f
        type: F &&
      - description: Arguments to be passed one-by-one to `f`
        name: args
        type: Args &&...
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <class F, class... Args>\nvoid for_each_argument(F && f, Args &&... args)"
namespace:
  - stlab
  - v1
---
