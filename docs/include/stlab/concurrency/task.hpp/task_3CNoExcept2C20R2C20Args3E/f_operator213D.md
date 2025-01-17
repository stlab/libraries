---
layout: function
title: operator!=
hyde:
  owner: sean-parent
  brief: __MISSING__
  tags:
    - function
  defined_in_file: stlab/concurrency/task.hpp
  overloads:
    auto operator!=(const task_<NoExcept, R, Args...> &, std::nullptr_t) -> bool:
      arguments:
        - description: __OPTIONAL__
          name: x
          type: const task_<NoExcept, R, Args...> &
        - description: __OPTIONAL__
          name: unnamed-1
          type: std::nullptr_t
          unnamed: true
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: auto operator!=(const task_<NoExcept, R, Args...> & x, std::nullptr_t) -> bool
    auto operator!=(std::nullptr_t, const task_<NoExcept, R, Args...> &) -> bool:
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: std::nullptr_t
          unnamed: true
        - description: __OPTIONAL__
          name: x
          type: const task_<NoExcept, R, Args...> &
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: auto operator!=(std::nullptr_t, const task_<NoExcept, R, Args...> & x) -> bool
  namespace:
    - stlab
    - v3
---
