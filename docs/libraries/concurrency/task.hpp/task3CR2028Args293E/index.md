---
layout: class
title: task<R (Args...)>
owner: sean-parent
brief: General-purpose polymorphic function wrapper.
tags:
  - class
defined_in_file: concurrency/task.hpp
declaration: "\nclass stlab::task;"
typedefs:
  result_type:
    definition: R
    description: __MISSING__
fields:
  _invoke:
    description: __MISSING__
    type: const stlab::task<R (Args...)>::invoke_t
  _vtable:
    description: __MISSING__
    type: const stlab::task<R (Args...)>::concept_t
namespace:
  - stlab
  - v1
---

Class template `stlab::task` is a general-purpose polymorphic function wrapper. Instances of `stlab::task` can store, move, and invoke any [Callable](http://en.cppreference.com/w/cpp/named_req/Callable) target -- functions, lambda expressions, bind expressions, or other function objects, as well as pointers to member functions and pointers to data members.

The stored callable object is called the target of `stlab::task`. If an `stlab::task` contains no target, it is called _empty_. Invoking the target of an empty `stlab::task` results in [`std::bad_function_call`](http://en.cppreference.com/w/cpp/utility/functional/bad_function_call) exception being thrown.

`stlab::task` is Similar to [`std::function`](http://en.cppreference.com/w/cpp/utility/functional/function) except it is not copyable and supports move-only and mutable callable targets which is especially useful for targets that may only be invoked once, often useful to deliver a message or payload in tasking or messaging system.

`stlab::task` satisfies the requirements of [MoveConstructible](http://en.cppreference.com/w/cpp/named_req/MoveConstructible) and [MoveAssignable](http://en.cppreference.com/w/cpp/named_req/MoveAssignable).