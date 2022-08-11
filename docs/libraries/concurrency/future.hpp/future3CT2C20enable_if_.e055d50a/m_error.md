---
layout: method
title: error
owner: sean-parent
brief: In case an error occurred, it returns the stored exception pointer.
tags:
  - method
defined_in_file: concurrency/future.hpp
overloads:
  stlab::optional<std::exception_ptr> error() const &:
    annotation:
      - deprecated (Use exception() instead)
    description: Iff an error occurred, returns the stored exception pointer.
    return: __OPTIONAL__
    signature_with_names: stlab::optional<std::exception_ptr> error() const &
---
Returns a potentially stored exception pointer. This function will be removed in future versions of the library. Use `exception()` instead.

