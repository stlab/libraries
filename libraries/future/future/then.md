---
layout: method
title: stlab::future
tags: library
full-name: stlab::future::then
pure-name: then
defined-in-header: stlab/future.hpp 
declaration: then()
description: Creates a new continuation on the current object.
entities:
  - kind: methods
    list:
      - name: stlab::future::then
        pure-name: then
        defined-in-header: stlab/future.hpp 
        declaration: |
          template <typename F> 
          auto then(F&& f) const&;
        description: Constructs a continuation using the same scheduler as this.
      - name: stlab::future::then
        pure-name: then
        defined-in-header: stlab/future.hpp 
        declaration: |
          template <typename S, typename F> 
          auto then(S&& s, F&& f) const&;
        description: Constructs a continuation using the provided scheduler.
      - name: stlab::future::then
        pure-name: then
        defined-in-header: stlab/future.hpp 
        declaration: |
          template <typename F>
          auto then(F&& f) &&;
        description: Constructs a continuation using the same scheduler as this.
      - name: stlab::future::then
        pure-name: then
        defined-in-header: stlab/future.hpp 
        declaration: |
          template <typename S, typename F>
          auto then(S&& s, F&& f) &&;
        description: Constructs a continuation using the provided scheduler.
  - kind: example
    code: NoCode
---
