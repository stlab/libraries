---
layout: class
title: buffer_size
owner: FelixPetriconi
brief: Encapsulates the buffer size of a process
tags:
  - class
defined_in_file: concurrency/channel.hpp
declaration: "\nstruct stlab::buffer_size;"
dtor: unspecified
fields:
  _value:
    description: __MISSING__
    type: std::size_t
namespace:
  - stlab
  - v1
---

The default size of an incomming queue of each process is 1. In case that one wants to change the queue size of a process, one can realize this by combining a `buffer_size` with a process by using an `operator&` before attaching it to a receiver. `buffer_size` of 0 means no limitation, beside the limitation of available memory. The purpose of the buffer size is flow control between processes. A processes will be allowed to run so long as it is yielding values until the buffer is full before it is suspended. If the processes awaits a value it may be suspended even if the buffer isn't full (if no value is available for the await). A buffer_size of 0 imposes no bounds. The default buffer_size is 1 (a value must be received before a process can yield the next value).