---
layout: method
title: stlab::immediate_executor
tags: [library]
full-name: stlab::immediate_executor::operator()
pure-name: operator()
defined-in-header: stlab/immediate_executor.hpp 
declaration: operator()
description: Executes the given function on this executor
entities:
  - kind: methods
    list:
      - name: stlab::immediate_executor::operator()
        pure-name: operator()
        defined-in-header: stlab/immediate_executor.hpp 
        declaration: template <typename F> void operator()(F&& f) const
        description: Execute the given function object immediately.
  - kind: example
    code: NoCode
---
