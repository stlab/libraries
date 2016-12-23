---
layout: method
title: stlab::future
tags: [library]
full-name: stlab::future::recover
pure-name: recover
defined-in-header: stlab/future.hpp 
declaration: recover()
description: Creates a recoverable continuation on the current object.
entities:
  - kind: methods
    list:
      - name: stlab::future::recover
        pure-name: recover
        defined-in-header: stlab/future.hpp 
        declaration: |
          template <typename F> 
          auto recover(F&& f) const&;
        description: Constructs a continuation using the same scheduler as this.
      - name: stlab::future::recover
        pure-name: recover
        defined-in-header: stlab/future.hpp 
        declaration: |
          template <typename S, typename F> 
          auto recover(S&& s, F&& f) const&;
        description: Constructs a continuation using the provided scheduler.
      - name: stlab::future::recover
        pure-name: recover
        defined-in-header: stlab/future.hpp 
        declaration: |
          template <typename F>
          auto recover(F&& f) &&;
        description: Constructs a continuation using the same scheduler as this.
      - name: stlab::future::recover
        pure-name: recover
        defined-in-header: stlab/future.hpp 
        declaration: |
          template <typename S, typename F>
          auto recover(S&& s, F&& f) &&;
        description: Constructs a continuation using the provided scheduler.
  - kind: example
    code: NoCode
---
