---
layout: method
title: operator()
owner: sean-parent
brief: Executes the given task after the specified duration.
tags:
  - method
defined_in_file: concurrency/system_timer.hpp
overloads:
  "template <typename F, typename Rep, typename Per>\nvoid operator()(std::chrono::duration<Rep, Per>, F) const":
    arguments:
      - description: The waiting duration after the task shall be executed.
        name: duration
        type: std::chrono::duration<Rep, Per>
      - description: __OPTIONAL__
        name: f
        type: F
    description: This executors executes the given task after the given duration.
    return: __OPTIONAL__
    signature_with_names: "template <typename F, typename Rep, typename Per>\nvoid operator()(std::chrono::duration<Rep, Per> duration, F f) const"
  "template <typename F>\nvoid operator()(std::chrono::steady_clock::time_point, F) const":
    annotation:
      - deprecated (Use chrono::duration as parameter instead)
    arguments:
      - description: The time point when the task shall be executed.
        name: when
        type: std::chrono::steady_clock::time_point
      - description: The task that shall be executed on the given executor.
        name: f
        type: F
    description: Executes the given task at the given time.
    return: __OPTIONAL__
    signature_with_names: "template <typename F>\nvoid operator()(std::chrono::steady_clock::time_point when, F f) const"
---

Executes the given task after the specified duration.

[deprecated] Executes the given task at the specified time point. 
When the time point is in the past, then the task will be executed immediately.

In case that the previous task takes longer as the time difference between its starting point and the start of the next task. Then the next task may be executed delayed. So a task can be executed later as specified, but never earlier.