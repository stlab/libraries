---
layout: method
title: error
tags: [library]
full-name: stlab::future::error
pure-name: error
defined-in-header: stlab/future.hpp 
declaration: error()
description: Returns a potentially stored exception pointer
entities:
  - kind: methods
    list:
      - name: stlab::future::error
        pure-name: error
        defined-in-header: stlab/future.hpp 
        declaration: boost::optional<std::exception_ptr> error() const
        description: In case an error occured, it returns the stored exception pointer.
  - kind: parameters
  - kind: result
    description: an optional that holds the exception pointer if an exception has occurred.
  - kind: example
    code: NoCode
---
