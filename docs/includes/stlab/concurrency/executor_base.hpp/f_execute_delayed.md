---
layout: function
title: execute_delayed
hyde:
  owner: sean-parent
  brief: __INLINED__
  tags:
    - function
  inline:
    brief:
      - returns an executor that will schedule the task to execute on the provided executor duration after it is invoked
  defined_in_file: stlab/concurrency/executor_base.hpp
  overloads:
    "template <typename E, typename Rep, typename Per>\nauto execute_delayed(std::chrono::duration<Rep, Per>, E)":
      arguments:
        - description: __OPTIONAL__
          name: duration
          type: std::chrono::duration<Rep, Per>
        - description: __OPTIONAL__
          name: executor
          type: E
      description: __INLINED__
      inline:
        description:
          - returns an executor that will schedule the task to execute on the provided executor duration after it is invoked
      return: __OPTIONAL__
      signature_with_names: "template <typename E, typename Rep, typename Per>\nauto execute_delayed(std::chrono::duration<Rep, Per> duration, E executor)"
  namespace:
    - stlab
    - v3
---
