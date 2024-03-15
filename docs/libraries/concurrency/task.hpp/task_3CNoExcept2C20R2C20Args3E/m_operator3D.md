---
layout: method
title: operator=
hyde:
  owner: __MISSING__
  brief: __MISSING__
  tags:
    - method
  defined_in_file: concurrency/task.hpp
  overloads:
    task_<NoExcept, R, Args...> & operator=(const task_<NoExcept, R, Args...> &):
      annotation:
        - deleted
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: const task_<NoExcept, R, Args...> &
          unnamed: true
      description: __OPTIONAL__
      return: __OPTIONAL__
      signature_with_names: task_<NoExcept, R, Args...> & operator=(const task_<NoExcept, R, Args...> &)
    task_<NoExcept, R, Args...> & operator=(std::nullptr_t):
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: std::nullptr_t
          unnamed: true
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: task_<NoExcept, R, Args...> & operator=(std::nullptr_t)
    task_<NoExcept, R, Args...> & operator=(task_<NoExcept, R, Args...> &&):
      arguments:
        - description: __OPTIONAL__
          name: x
          type: task_<NoExcept, R, Args...> &&
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: task_<NoExcept, R, Args...> & operator=(task_<NoExcept, R, Args...> && x)
    "template <class F>\nauto operator=(F &&) -> std::enable_if_t<!NoExcept || std::is_nothrow_invocable_v<decltype(f), Args...>, task_<NoExcept, R, Args...> &>":
      arguments:
        - description: __OPTIONAL__
          name: f
          type: F &&
      description: __MISSING__
      return: __OPTIONAL__
      signature_with_names: "template <class F>\nauto operator=(F && f) -> std::enable_if_t<!NoExcept || std::is_nothrow_invocable_v<decltype(f), Args...>, task_<NoExcept, R, Args...> &>"
---