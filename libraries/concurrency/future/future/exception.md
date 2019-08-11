---
layout: method
title: exception
tags: [library]
scope: future
pure-name: exception
defined-in-header: stlab/concurrency/future.hpp 
declaration: exception()
description: Returns a potentially stored exception pointer. 
entities:
  - kind: methods
    list:
      - name: stlab::future::exception
        pure-name: exception
        defined-in-header: stlab/concurrency/future.hpp 
        declaration: std::exception_ptr exception() const&
        description: In case an error occurred, it returns the stored exception pointer.
  - kind: result
    description: The exception pointer if an exception has occurred.
---






