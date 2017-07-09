---
layout: class
title: stlab::buffer_size
tags: [library]
pure-name: buffer_size
dtor: default
name: stlab::buffer_size
defined-in-header: stlab/concurrency/channel.hpp
declaration: struct buffer_size
brief: Encapsulates the buffer size of a process
description: The default size of an incomming queue of each process has no limitation, beside the limitation of available memory. In case that one wants to limit the queue size of a process, one can realize this by combining a `buffer_size` with a process by using an `operator&` before attaching it to a receiver.
---
