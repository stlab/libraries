---
layout: function
title: stlab::for_each_argument
tags: [library]
scope: stlab
pure-name: for_each_argument
brief: Execute a function for each passed argument
annotation: template function
example: [ for_each_argument_example.cpp ]
defined-in-header: stlab/utility.hpp
entities:
  - kind: overloads
    list:
      - name: for_each_argument
        pure-name: for_each_argument
        declaration: |
            template<class F, class... Args>
            void for_each_argument(F&& f, Args&&...args)
        description: Executes `f` individually for each of `args`
  - kind: parameters
    list:
      - name: f
        description: Unary function that can take all arguments of type `Args`
      - name: args
        description: Arguments to be passed one-by-one to `f`
---
