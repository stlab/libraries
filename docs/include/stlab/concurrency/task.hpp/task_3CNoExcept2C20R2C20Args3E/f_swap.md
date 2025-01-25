---
layout: function
title: swap
hyde:
  owner: sean-parent
  brief: __MISSING__
  tags:
    - function
  defined_in_file: stlab/concurrency/task.hpp
  overloads:
    void swap(task_<NoExcept, R, Args...> &, task_<NoExcept, R, Args...> &):
      arguments:
        - description: __OPTIONAL__
          name: x
          type: task_<NoExcept, R, Args...> &
        - description: __OPTIONAL__
          name: y
          type: task_<NoExcept, R, Args...> &
      description: __OPTIONAL__
      return: __OPTIONAL__
      signature_with_names: void swap(task_<NoExcept, R, Args...> & x, task_<NoExcept, R, Args...> & y)
  namespace:
    - stlab
    - v3
---
