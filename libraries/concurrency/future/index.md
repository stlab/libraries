---
layout: class
title: stlab::future
tags: [library]
entities:
  - kind: class
    name: stlab::future
    pure-name: future
    defined-in-header: stlab/future.hpp
    git-link: https://github.com/stlab/libraries/blob/develop/stlab/future.hpp
    declaration: |
      template <typename T>
      class future
    description: |
      `stlab::future` differs from `std::future` in several ways:
      
      1. This future is copyable, so there is no need for a `std::shared_future`.
      1. If this future is only used as an rvalue and there are no copies, then the value (either returned by `get_try` or through a continuation) will be moved. 
      1. Multiple continutations - forming a split - may be attached to a single future with `then()`. `then()` is declared `const` since it does not mutate the result object of the future. The continuation is called with the value type, not the future. 
      1. A sink argument to a continuation should take the argument by value and move the object as needed. 
      1. If the continuation reads the argument it should take it by `const&`. 
      1. Behavior of modifying the argument through a non-const reference is undefined (may be a compilation error). 
      1. If the last copy of a future destructs, the associated task and any held futures for the task arguments are released and the associated packaged_task will become a no-op if called. 
      1. There are no `wait()` or `get()` functions. Instead, there is a `get_try()` which returns an `optional<T>` (or if `T` is `void`, the result is a `bool` with `true` indicating the associated task has executed.

      ## To Do
      - Error handling
    member-types:
      - type: value_type
        definition: T
    member-functions:
      - name: valid
        description: Returns `true` if the future is valid
      - name: then
        description: Creates a continuation
      - name: recover
        description: Creates a recovering continuation
      - name: detach
        description: Detaches the current future
      - name: reset
        description: Cancels this future
      - name: get_try
        description: Tries to access the futures result
      - name: error
        description: Returns a possibly stored exception
  - kind: free-functions
    functions:
      - name: make_exceptional_future
        defined-in-header: stlab/utility.hpp
        description: Creates a future that is already ready.
        pure-name: make_ready_future
      - name: make_ready_future
        defined-in-header: stlab/utility.hpp
        description: Creates a future that is already ready.
        pure-name: make_ready_future
      - name: when_all
        defined-in-header: stlab/future.hpp
        description: Joins futures
        pure-name: when_all
      - name: when_any
        defined-in-header: stlab/future.hpp
        description: Reduces (correct name?) several futures into a single one
        pure-name: when_any
---


### Single Future Example ###


