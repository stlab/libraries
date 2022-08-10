---
layout: function
title: swap
owner: sean-parent
brief: Swaps an other task with this
tags:
  - function
defined_in_file: concurrency/task.hpp
overloads:
  void swap(task<R (Args...)> &, task<R (Args...)> &):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: task<R (Args...)> &
      - description: __OPTIONAL__
        name: y
        type: task<R (Args...)> &
    description: Exchanges this task with the passed one.
    return: __OPTIONAL__
    signature_with_names: void swap(task<R (Args...)> & x, task<R (Args...)> & y)
namespace:
  - stlab
  - v1
---
