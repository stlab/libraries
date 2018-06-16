---
layout: method
title: error
tags: [library]
scope: future
pure-name: error
defined-in-header: stlab/concurrency/future.hpp 
declaration: error()
description: Returns a potentially stored exception pointer
entities:
  - kind: methods
    list:
      - name: stlab::future::error
        pure-name: error
        defined-in-header: stlab/concurrency/future.hpp 
        declaration: stlab::optional<std::exception_ptr> error() const
        description: In case an error occurred, it returns the stored exception pointer.
  - kind: result
    description: an optional that holds the exception pointer if an exception has occurred.
---
