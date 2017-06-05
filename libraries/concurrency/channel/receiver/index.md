---
layout: class
title: stlab::receiver
tags: [library]
name: stlab::receiver
pure-name: receiver
defined-in-header: stlab/channel.hpp
git-link: https://github.com/stlab/libraries/blob/develop/stlab/channel.hpp
ctor: default
dtor: default
declaration: |
    template <typename T> 
    class receiver
brief: Implements the receiving part of a CSP channel
description: |
  This class implements the receiving part of a [CSP](https://en.wikipedia.org/wiki/Communicating_sequential_processes) channel. Each receiver has an attached process that gets executed when a value is send through the sender into the channel. This attached process must either be an n-ary function object - n depends on the number of attached upstream receiver - or it must of a type that implements an `await()` and `yield()` method.
  
  Multiple calls with this operator on the same object realize a split. That means that the result of this process is copied into all attached downstream channels.
  
  In case that `T` of `receiver<T>` is a move only type, repeated calls of this operator overwrite the previous attached downstream channel.
  
  If an exception is thrown while calling the attached process, the exception pointer is passed to the attached process, if it has a `set_error()` method, otherwise this process is closed.
member-types:
  - type: value_type
    definition: T
---
