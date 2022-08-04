---
layout: class
title: process
tags: [library]
ctor: unspecified
dtor: unspecified
annotation: policy class
kind: class
name: process
pure-name: process
brief: Describes a process that gets executed whenever a value is passed into the channel
description: |
  The process is a policy that must follow certain rules; it is not a type that the library provides.

  Each channel receiver is associated with a process. The process can either be:

    - an n-ary function object, further here called a _function-process_. 
    - a type that must have properties, that are described below. It is further here called an _await-process_.

  Since a function-process does not need further explication, the following concentrates on await-processes.
  The process accepts values via `await()` as long as the `state()` returns await. If the state changes to yield
  then the associated receiver will call `yield()`. 
  For a further details see the [`state()`](state.html) method.

  It is ensured with a static assert that the process implements a `state() const` method.
example: process_example.cpp
---

### Signature of an await-process ###

Here it is assumed that the process takes `T...`as argument and its result type is `U`.

~~~ c++
struct process 
{
    void await(T... val);

    U yield();

    void close();                             // optional

    void set_error(std::exception_ptr error); // optional

    process_state_scheduled state() const;
};
~~~
