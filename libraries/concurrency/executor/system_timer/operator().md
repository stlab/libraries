---
layout: method
title: stlab::system_timer
tags: [library]
pure-name: operator()
defined-in-header: stlab/concurrency/system_timer.hpp 
description: |
  Executes the given task after the specified duration.

  [deprecated] Executes the given task at the specified time point. 
  When the time point is in the past, then the task will be executed immediately.

  In case that the previous task takes longer as the time difference between its starting point and the start of the next task. Then the next task may be executed delayed. So a task can be executed later as specified, but never earlier.
entities:
  - kind: methods
    list:
      - name: stlab::system_timer::operator()
        pure-name: operator()
        defined-in-header: stlab/concurrency/system_timer.hpp 
        declaration: |
          template <typename F>
          [[deprecated]] void operator() (std::chrono::steady_clock::time_point when, F&& f) const
        description: This executors executes the given task at the given time.
      - name: stlab::system_timer::operator()
        pure-name: operator()
        defined-in-header: stlab/concurrency/system_timer.hpp 
        declaration: |
          template <typename F, typename Rep, typename Per = std::ratio<1>>
          void operator()(std::chrono::duration<Rep, Per> duration, F f) const
        description: This executors executes the given task after the given duration.
  - kind: parameters
    list:
      - name: duration
        description: The waiting duration after the task shall be executed.
      - name: when
        description: The time point when the task shall be executed.
      - name: f
        description: The task that shall be executed on the given executor.
---
