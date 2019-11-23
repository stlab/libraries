---
layout: function
title: error
brief: In case an error occurred, it returns the stored exception pointer.
defined-in-header: stlab/concurrency/future.hpp 
tags:
  - function
overloads:
  stlab::optional<std::exception_ptr> error():
    description: In case an error occurred, it returns the stored exception pointer.
    return: __OPTIONAL__
    signature_with_names: "[[deprecated]] stlab::optional<std::exception_ptr> error() const"
---
Returns a potentially stored exception pointer. This function will be removed in future versions of the library. Use `exception()` instead.

