---
layout: class
title: stlab::task
tags: [library]
scope: stlab
pure-name: task
defined-in-header: stlab/concurrency/task.hpp
git-link: https://github.com/stlab/libraries/blob/develop/stlab/concurrency/task.hpp
dtor: default
annotation: template class
declaration: |
  template <typename F>
  class task
brief: A type-erased nullary wrapper that supports mutable functions.
description: Tasks are movable, not copyable.
---
