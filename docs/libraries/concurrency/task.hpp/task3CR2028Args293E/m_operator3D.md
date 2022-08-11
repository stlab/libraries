---
layout: method
title: operator=
owner: sean-parent
brief: Move assignment operator
tags:
  - method
defined_in_file: concurrency/task.hpp
overloads:
  task<R (Args...)> & operator=(const task<R (Args...)> &):
    annotation:
      - delete
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: const task<R (Args...)> &
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: task<R (Args...)> & operator=(const task<R (Args...)> &)
  task<R (Args...)> & operator=(std::nullptr_t):
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: std::nullptr_t
        unnamed: true
    description: Destroys the current function and creates a new empty one.
    return: __OPTIONAL__
    signature_with_names: task<R (Args...)> & operator=(std::nullptr_t)
  task<R (Args...)> & operator=(task<R (Args...)> &&):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: task<R (Args...)> &&
    description: Moves the given task `x` into `this`.
    return: __OPTIONAL__
    signature_with_names: task<R (Args...)> & operator=(task<R (Args...)> && x)
  "template <class F>\ntask<R (Args...)> & operator=(F &&)":
    arguments:
      - description: __OPTIONAL__
        name: f
        type: F &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <class F>\ntask<R (Args...)> & operator=(F && f)"
---
