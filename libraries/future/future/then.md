---
layout: method
title: stlab::future
tags: [library]
full-name: stlab::future::get_try
pure-name: get_try
defined-in-header: stlab/future.hpp 
declaration: get_try()
description: Returns the result of the future.
entities:
  - kind: methods
    list:
      - name: stlab::future::get_try
        pure-name: get_try
        defined-in-header: stlab/future.hpp 
        declaration: boost::optional<T> get_try() const&;
        description: If T is a not void it returns a set optional if the future succeeded, otherwise an empty optional. In case that
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
