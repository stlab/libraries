---
layout: function
title: operator co_await
brief: Creates a awaitable object on a given future.
defined-in-header: stlab/concurrency/future.hpp
tags:
  - function
example: operator_co_await_example.cpp
overloads:
  "template <typename R>\nauto operator co_await(stlab::future<R>&&)":
    description: Creates an awaitable object on a given future of a move-only or copyable type R.
    arguments:
      - description: The value(s) that are passed to `await`
        name: x
        type: T
    return: __OPTIONAL__
    signature_with_names: "template <typename R>\nauto operator co_await(stlab::future<R>&& x)"
  "auto operator co_await(stlab::future<void>&&)":
    description: Creates an awaitable object on a given future of type void.
    arguments:
      - description: The value(s) that are passed to `await`
        name: x
        type: T
    return: __OPTIONAL__
    signature_with_names: "auto operator co_await(stlab::future<void>&& x)"
---
This operator converts the given future to an awaitable objects for a coroutine. 
