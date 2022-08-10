---
layout: function
title: execute_delayed
owner: __MISSING__
brief: __MISSING__
tags:
  - function
defined_in_file: concurrency/executor_base.hpp
overloads:
  "template <typename E, typename Rep, typename Per>\nauto execute_delayed(std::chrono::duration<Rep, Per>, E)":
    arguments:
      - description: __OPTIONAL__
        name: duration
        type: std::chrono::duration<Rep, Per>
      - description: __OPTIONAL__
        name: executor
        type: E
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <typename E, typename Rep, typename Per>\nauto execute_delayed(std::chrono::duration<Rep, Per> duration, E executor)"
namespace:
  - stlab
  - v1
---
