---
layout: method
title: stlab::default_executor
tags: [library]
full-name: stlab::default_executor::operator()
pure-name: operator()
defined-in-header: stlab/default_executor.hpp 
declaration: operator()
description: Executes the given function on this executor
entities:
  - kind: methods
    list:
      - name: stlab::default_executor::operator()
        pure-name: operator()
        defined-in-header: stlab/default_executor.hpp 
        declaration: template <typename F> void operator()(F&& f) const
        description: Pushes the given function object on the systems thread pool.
  - kind: example
    code: NoCode
---
