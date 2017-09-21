---
layout: class
title: stlab::task
tags: [library]
scope: stlab
pure-name: task
defined-in-header: stlab/concurrency/task.hpp
dtor: default
annotation: template class
declaration: |
  template <typename F>
  class task
brief: A type-erased nullary wrapper that supports mutable functions.
description: Tasks are functions with a mutable call operator to support moving items through for single
    invocations. Tasks are movable, not copyable.
---
