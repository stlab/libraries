---
layout: class
title: receiver<T>
owner: FelixPetriconi
brief: Implements the receiving part of a CSP channel
tags:
  - class
defined_in_file: concurrency/channel.hpp
declaration: "template <typename T>\nclass stlab::receiver;"
typedefs:
  result_type:
    definition: T
    description: __MISSING__
namespace:
  - stlab
  - v1
---

Each receiver has an attached process that gets executed when a value is send through the sender into the channel. This attached process must either be an n-ary function object - n depends on the number of attached upstream receiver - or it must be of a type that implements an `await()` and `yield()` method.

Multiple calls to `operator|` on the same object split the channel. That means that the result of this process is copied into all attached downstream channels.

The queue size of the attached process can be limited with a `buffer_size`.

If an exception is thrown while calling the attached process, the exception pointer is passed to the attached process, if it has a `set_error()` method, otherwise this process is closed.

All non-destructed receivers on a channel must be `set_ready()` or data cannot flow to any
stream processes.

It is necessary to specialize the template `stlab::smart_test` if type T is e.g. a container of a non copyable type like `std::vector<std::unique_ptr<>>` so that the the library can correctly dispatch for move-only types internally. This is unfortunately necessary because of a defect in the C++ standard. (The trait std::is_copy_constructible<> does not return the correct results in such a case.)