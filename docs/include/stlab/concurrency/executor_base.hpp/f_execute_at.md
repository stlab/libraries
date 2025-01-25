---
layout: function
title: execute_at
hyde:
  owner: sean-parent
  brief: __INLINED__
  tags:
    - function
  inline:
    brief: _multiple descriptions_
  defined_in_file: stlab/concurrency/executor_base.hpp
  overloads:
    auto execute_at(std::chrono::duration<long, ratio<1, 1000000000>>, stlab::executor_t) -> stlab::executor_t:
      arguments:
        - description: __OPTIONAL__
          name: duration
          type: std::chrono::duration<long, ratio<1, 1000000000>>
        - description: __OPTIONAL__
          name: executor
          type: stlab::executor_t
      description: __INLINED__
      inline:
        description:
          - returns an executor that will schedule any passed task to it to execute at the given time point on the executor provided
      return: __OPTIONAL__
      signature_with_names: auto execute_at(std::chrono::duration<long, ratio<1, 1000000000>> duration, stlab::executor_t executor) -> stlab::executor_t
    auto execute_at(std::chrono::steady_clock::time_point, stlab::executor_t) -> stlab::executor_t:
      annotation:
        - deprecated("Use chrono::duration as parameter instead")
      arguments:
        - description: __OPTIONAL__
          name: when
          type: std::chrono::steady_clock::time_point
        - description: __OPTIONAL__
          name: executor
          type: stlab::executor_t
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: auto execute_at(std::chrono::steady_clock::time_point when, stlab::executor_t executor) -> stlab::executor_t
    "template <typename Rep, typename Per>\nauto execute_at(std::chrono::duration<Rep, Per>, stlab::executor_t) -> stlab::executor_t":
      arguments:
        - description: __OPTIONAL__
          name: duration
          type: std::chrono::duration<Rep, Per>
        - description: __OPTIONAL__
          name: executor
          type: stlab::executor_t
      description: __INLINED__
      inline:
        description:
          - returns an executor that will schedule any passed task to it to execute at the given time point on the executor provided
      return: __OPTIONAL__
      signature_with_names: "template <typename Rep, typename Per>\nauto execute_at(std::chrono::duration<Rep, Per> duration, stlab::executor_t executor) -> stlab::executor_t"
  namespace:
    - stlab
    - v3
---
