---
layout: method
title: exception
owner: sean-parent
brief: Returns a potentially stored exception pointer.
tags:
  - method
defined_in_file: concurrency/future.hpp
overloads:
  std::exception_ptr exception() const &:
    description: Iff an error occurred, it returns the stored exception pointer.
    return: The exception pointer if an exception has occurred.
    signature_with_names: std::exception_ptr exception() const &
---
