---
layout: class
title: stlab::task
tags: [library]
scope: stlab
pure-name: task
defined-in-header: stlab/concurrency/task.hpp
dtor: default
annotation: template class
declaration:
    - |
        template <class>
        class task; /* undefined */
    - |
        template <class R, class... Args>
        class task<R(Args...)>
brief: A type-erased nullary wrapper that supports mutable functions.
description: Tasks are movable, not copyable.
member-types:
    - type: result_type
      definition: R
---
