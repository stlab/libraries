---
layout: method
title: task_<NoExcept, R, Args...>
hyde:
  owner: sean-parent
  brief: __OPTIONAL__
  tags:
    - method
  defined_in_file: stlab/concurrency/task.hpp
  is_ctor: true
  overloads:
    task_<NoExcept, R, Args...>():
      annotation:
        - defaulted
      description: __OPTIONAL__
      signature_with_names: task_<NoExcept, R, Args...>()
    task_<NoExcept, R, Args...>(const task_<NoExcept, R, Args...> &&):
      annotation:
        - deleted
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: const task_<NoExcept, R, Args...> &&
          unnamed: true
      description: __OPTIONAL__
      signature_with_names: task_<NoExcept, R, Args...>(const task_<NoExcept, R, Args...> &&)
    task_<NoExcept, R, Args...>(const task_<NoExcept, R, Args...> &):
      annotation:
        - deleted
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: const task_<NoExcept, R, Args...> &
          unnamed: true
      description: __OPTIONAL__
      signature_with_names: task_<NoExcept, R, Args...>(const task_<NoExcept, R, Args...> &)
    task_<NoExcept, R, Args...>(std::nullptr_t):
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: std::nullptr_t
          unnamed: true
      description: __MISSING__
      signature_with_names: task_<NoExcept, R, Args...>(std::nullptr_t)
    task_<NoExcept, R, Args...>(task_<NoExcept, R, Args...> &&):
      arguments:
        - description: __OPTIONAL__
          name: x
          type: task_<NoExcept, R, Args...> &&
      description: __MISSING__
      signature_with_names: task_<NoExcept, R, Args...>(task_<NoExcept, R, Args...> && x)
    "template <class F, std::enable_if_t<!NoExcept || std::is_nothrow_invocable_v<F, Args...>, bool> >\ntask_<NoExcept, R, Args...>(F &&)":
      arguments:
        - description: __OPTIONAL__
          name: f
          type: F &&
      description: __MISSING__
      signature_with_names: "template <class F, std::enable_if_t<!NoExcept || std::is_nothrow_invocable_v<F, Args...>, bool> >\ntask_<NoExcept, R, Args...>(F && f)"
---
