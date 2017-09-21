---
layout: class
title: stlab::receiver
tags: [library]
name: stlab::receiver
pure-name: receiver
defined-in-header: stlab/concurrency/channel.hpp
ctor: default
dtor: default
annotation: template class
declaration: |
    template <typename T> 
    class receiver
brief: Implements the receiving part of a CSP channel
description: |
  Each receiver has an attached process that gets executed when a value is send through the sender into the channel. This attached process must either be an n-ary function object - n depends on the number of attached upstream receiver - or it must of a type that implements an `await()` and `yield()` method.
  
  Multiple calls with this operator on the same object realize a split. That means that the result of this process is copied into all attached downstream channels.
  
  The queue size of the attached process can be limited with a `buffer_size`.

  If an exception is thrown while calling the attached process, the exception pointer is passed to the attached process, if it has a `set_error()` method, otherwise this process is closed.
member-types:
  - type: value_type
    definition: T
---
