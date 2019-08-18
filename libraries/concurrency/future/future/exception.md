---
layout: function
title: exception
brief: Returns a potentially stored exception pointer. 
defined-in-header: stlab/concurrency/future.hpp 
tags:
  - function
overloads:
  std::exception_ptr exception():
    desciption: In case an error occurred, it returns the stored exception pointer.
    return: The exception pointer if an exception has occurred.
    siganture_with_names: std::exception_ptr exception() const
---
