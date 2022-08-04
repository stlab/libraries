---
layout: method
title: stlab::task::task
tags: [library]
scope: task
pure-name: task
defined-in-header: stlab/concurrency/task.hpp
brief: Constructs an `stlab::task` from a variety of sources.
description: |
    When the target is a function pointer or std::reference_wrapper, small object optimization is
    guaranteed, that is, these targets are always directly stored inside the `stlab::task` object,
    no dynamic allocation takes place. Other large objects may be constructed in dynamic allocated
    storage and accessed by the `stlab::task` object through a pointer.
entities:
  - kind: methods
    list:
      - name: stlab::task::task
        pure-name: task
        defined-in-header: stlab/concurrency/task.hpp
        declaration: |
            constexpr task() noexcept;
        description: Creates an _empty_ task.
      - name: stlab::task::task
        pure-name: task
        defined-in-header: stlab/concurrency/task.hpp
        declaration: |
            constexpr task(std::nullptr_t) noexcept;
        description: Creates an _empty_ task.
      - name: stlab::task::task
        pure-name: task
        defined-in-header: stlab/concurrency/task.hpp
        declaration: |
            task(const task&) = delete;
        description: Copying of a task is disabled
      - name: stlab::task::task
        pure-name: task
        defined-in-header: stlab/concurrency/task.hpp
        declaration: |
            task(task&& other) noexcept;
        description: >
            Moves the _target_ of `other` to the target of `*this`. If `other` is _empty_, `*this`
            will be _empty_ after the call too. `other` is in a partially formed state after the
            call.
      - name: stlab::task::task
        pure-name: task
        defined-in-header: stlab/concurrency/task.hpp
        declaration: |
            template <class F>
            task(F&& f);
        description: >
            Initializes the _target_ with a copy of `f`. If `f` is a null pointer to function, null
            pointer to member, or _empty_ `std::function<R(Args...)>`, then `*this` will be _empty_
            after the call. [_To be added for C++17._ This constructor does not participate in
            overload resolution unless `f` is
            [Callable](http://en.cppreference.com/w/cpp/named_req/Callable) for argument types
            `Args...` and return type `R`.]
  - kind: parameters
    list:
      - name: other
        description: >
          a task object used to initialize `*this`.
      - name: f
        description: >
          a callable used to initialize `*this`.
---
