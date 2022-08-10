---
layout: function
title: operator!=
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: concurrency/task.hpp
overloads:
  bool operator!=(const task<R (Args...)> &, std::nullptr_t):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: const task<R (Args...)> &
      - description: __OPTIONAL__
        name: unnamed-1
        type: std::nullptr_t
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: bool operator!=(const task<R (Args...)> & x, std::nullptr_t)
  bool operator!=(std::nullptr_t, const task<R (Args...)> &):
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: std::nullptr_t
        unnamed: true
      - description: __OPTIONAL__
        name: x
        type: const task<R (Args...)> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: bool operator!=(std::nullptr_t, const task<R (Args...)> & x)
namespace:
  - stlab
  - v1
---
