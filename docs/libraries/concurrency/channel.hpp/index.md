---
layout: library
title: concurrency/channel.hpp
owner: sean-parent
brief: This library implements the channel model of interprocess communication.
tags:
  - sourcefile
library-type: sourcefile
typedefs:
  argument_of_t:
    definition: typename argument_of<T>::type
    description: __MISSING__
  first_t:
    definition: typename first_<T...>::type
    description: __MISSING__
  process_state_scheduled:
    definition: std::pair<process_state, std::chrono::nanoseconds>
    description: __MISSING__
  result_of_t_:
    definition: typename result_of_<F>::type
    description: __MISSING__
---

[Channels](https://en.wikipedia.org/wiki/Channel_(programming)) are one model of interprocess communication that has its roots in [communicating sequential process](https://en.wikipedia.org/wiki/Communicating_sequential_processes) (CSP).
With channels it is possible to realize processing graphs, that can be used for more than one execution, compared to a graph that is setup with futures.
Channels consist of a sending and a receiving part. Each receiver has an attached process that gets executed when a value is send through. It is possible to split, zip, zip_with, or merge channels. Each processing node is associated with a [process](process/index.html).