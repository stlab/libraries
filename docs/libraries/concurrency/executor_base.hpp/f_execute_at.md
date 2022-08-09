---
layout: function
title: execute_at
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: concurrency/executor_base.hpp
overloads:
  stlab::executor_t execute_at(std::chrono::duration<long long, ratio<1, 1000000000>>, stlab::executor_t):
    arguments:
      - description: __OPTIONAL__
        name: duration
        type: std::chrono::duration<long long, ratio<1, 1000000000>>
      - description: __OPTIONAL__
        name: executor
        type: stlab::executor_t
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: stlab::executor_t execute_at(std::chrono::duration<long long, ratio<1, 1000000000>> duration, stlab::executor_t executor)
  stlab::executor_t execute_at(std::chrono::steady_clock::time_point, stlab::executor_t):
    annotation:
      - deprecated (Use chrono::duration as parameter instead)
    arguments:
      - description: __OPTIONAL__
        name: when
        type: std::chrono::steady_clock::time_point
      - description: __OPTIONAL__
        name: executor
        type: stlab::executor_t
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: stlab::executor_t execute_at(std::chrono::steady_clock::time_point when, stlab::executor_t executor)
  "template <typename Rep, typename Per>\nstlab::executor_t execute_at(std::chrono::duration<Rep, Per>, stlab::executor_t)":
    arguments:
      - description: __OPTIONAL__
        name: duration
        type: std::chrono::duration<Rep, Per>
      - description: __OPTIONAL__
        name: executor
        type: stlab::executor_t
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: "template <typename Rep, typename Per>\nstlab::executor_t execute_at(std::chrono::duration<Rep, Per> duration, stlab::executor_t executor)"
namespace:
  - stlab
  - v1
---
