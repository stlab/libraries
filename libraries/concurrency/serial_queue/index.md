
---
layout: class
title: stlab::serial_queue
tags: [library]
scope: stlab
pure-name: serial_queue
defined-in-header: stlab/concurrency/serial_queue.hpp
git-link: https://github.com/stlab/libraries/blob/develop/stlab/concurrency/serial_queue.hpp
dtor: default
annotation: template class
declaration: |
  template <typename E>
  class serial_queue
brief: An executor wrapper that causes enqueued tasks to be run serially.
description: |
  
member-types:
  - type: executor_t
    definition: E
---
