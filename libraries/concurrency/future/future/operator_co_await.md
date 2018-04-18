---
layout: method
title: operator co_await
tags: [library]
scope: future
pure-name: operator co_await
defined-in-header: stlab/concurrency/future.hpp 
declaration: operator co_await()
brief: Creates a awaitable object on a given future.
description: This operator converts the given future to an awaitable objects for a coroutine. 
example: operator_co_await_example.cpp
entities:
  - kind: methods
    list:
      - name: stlab::future::operator co_await
        pure-name: operator co_await
        defined-in-header: stlab/concurrency/future.hpp 
        declaration: |
          template <typename R>
          auto operator co_await(stlab::future<R> &&f)
        description: Creates a awaitable object on a given future of a move-only or copyable type R.
      - name: stlab::future::operator co_await
        pure-name: operator co_await
        defined-in-header: stlab/concurrency/future.hpp 
        declaration: |
          auto operator co_await(stlab::future<void> &&f)
        description: Creates an awaitable object on a given future of type void.
  - kind: parameters
    list:
      - name: f
        description: The future that shall be awaitable.
  - kind: result
    description: The awaitable object
---

