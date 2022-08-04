---
layout: function
title: future
brief: Constructs a future
defined-in-header: stlab/concurrency/future.hpp
tags:
  - function
overloads:
  future<T>():
    description: The first call to the default constructor will construct an empty future.
    return: __OPTIONAL__
    signature_with_names: future<T>()
  future<T>(const future<T> &):  
    description: The copy constructor is available for futures of copyable T and void. It is disabled for move only types
    arguments:
      - description: __OPTIONAL__
        name: x
        type: const future<T> &
    return: __OPTIONAL__
    signature_with_names: future<T>(const future<T>& x)
  future<T>(future<T>&& ):
    description: |
            Moves the _target_ of `x` to the target of `*this`. If `x` is _empty_, `*this`
            will be _empty_ after the call too. `x` is in a partially formed state after the
            call.
    arguments:
      - description: __OPTIONAL__
        name: x
        type: future<T>&&
    return: __OPTIONAL__
    signature_with_names: future<T>(future<T>&& x) noexept
  future<T>& operator=(const future<T>& ): 
    description: The copy assignment operator is available for copyable T and void futures. It is disabled for move only types.
    arguments:
      - description: __OPTIONAL__
        name: x
        type: const future<T>&
    return: __OPTIONAL__
    signature_with_names: future<T>& operator=(const future<T>& x)
  future<T>& operator=(future<T>&& ):
    description: |
            Moves the _target_ of `x` to the target of `*this`. If `x` is _empty_, `*this`
            will be _empty_ after the call too. `x` is in a partially formed state after the
            call.
    arguments:
      - description: __OPTIONAL__
        name: x
        type: future<T>&&
    return: __OPTIONAL__
    signature_with_names: future<T>& operator=(future<T>&& x) noexept
---

