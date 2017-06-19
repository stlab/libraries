---
layout: method
title: stlab::system_timer
tags: [library]
pure-name: operator()
defined-in-header: stlab/concurrency/system_timer.hpp 
description: |
  Executes the given function at the specified time point. 
  
  When the time point is in the past, then the function will be executed immediately.

  In case that the previous function takes longer as the time difference between its starting point and the start of the next function. Then the next function may be executed delayed. So a function can be executed later as specified, but never earlier.
entities:
  - kind: methods
    list:
      - name: stlab::system_timer::operator()
        pure-name: operator()
        defined-in-header: stlab/concurrency/system_timer.hpp 
        declaration: |
          template <typename F>
          void operator() (std::chrono::system_clock::time_point when, F&& f) const
        description: This executors executes the given function.
  - kind: parameters
    list:
      - name: when
        description: The time point when the function shall be executed.
      - name: f
        description: The function object that shall be executed on the main loop.
---
