---
layout: method
title: stlab::future::future
tags: [library]
scope: future
pure-name: future
defined-in-header: stlab/concurrency/future.hpp
brief: Constructs a future
entities:
  - kind: methods
    list:
      - name: stlab::concurrency::future
        pure-name: future
        declaration: |
            future()
        description: The first call to the default constructor will construct an empty future.
      - name: stlab::concurrency::future
        pure-name: future
        declaration: |
            future(const future& other) = default
        description: The copy constructor is available for futures of copyable T and void.
      - name: stlab::concurrency::future
        pure-name: future
        declaration: |
            future(const future& other) = disabled
        description: The copy constructor is disabled for move only types.      
      - name: stlab::concurrency::future
        pure-name: future
        declaration: |
            future(future&& other) noexcept
        description: |
            Moves the _target_ of `other` to the target of `*this`. If `other` is _empty_, `*this`
            will be _empty_ after the call too. `other` is in a partially formed state after the
            call.
      - name: stlab::future::operator=
        pure-name: operator=
        defined-in-header: stlab/concurrency/future.hpp 
        declaration: |
            future& operator=(const future& x) = default
        description: The copy assignment operator is available for copyable T and void futures
      - name: stlab::future::operator=
        pure-name: operator=
        defined-in-header: stlab/concurrency/future.hpp 
        declaration: |
            future& operator=(const future& x) = delete
        description: The copy assignment operator is disabled for move only types.
      - name: stlab::concurrency::future
        pure-name: future
        declaration: |
            future& operator=(future&& other) noexcept
        description: |
            Moves the _target_ of `other` to the target of `*this`. If `other` is _empty_, `*this`
            will be _empty_ after the call too. `other` is in a partially formed state after the
            call.
---



---
layout: function
title: await
brief: Called on an await-process whenever a new value was received from upstream.
tags:
  - function
example: await_example.cpp
overloads:
  void await(T... val):
    description: This method is called with the next set of values
    arguments:
      - description: The value(s) that are passed to `await`
        name: val
        type: T
    return: __OPTIONAL__
    signature_with_names: void await(T... val)
---
This method is called on an await-process whenever a new value was received from upstream. The type `T` stands here for any semi regular or move-only type. The number of arguments depends on the number of attached upstream sender. Potential state changes from awaitable to yieldable should happen while this method is invoked.
