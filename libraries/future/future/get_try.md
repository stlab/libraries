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
        description: If T is a not void it returns a set optional if the future succeeded, otherwise an empty optional. In case that an error occured it rethrows the captured exception.
      - name: stlab::future::then
        pure-name: then
        defined-in-header: stlab/future.hpp 
        declaration: bool get_try() const&
        description: If T is void it returns true if the future succeeded, otherwise false. In case that an error occured it rethrows the captured exception.
  - kind: example
    code: NoCode
---