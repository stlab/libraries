---
layout: method
title: operator=
hyde:
  owner: sean-parent
  brief: __OPTIONAL__
  tags:
    - method
  defined_in_file: stlab/concurrency/future.hpp
  overloads:
    auto operator=(const packaged_task<Args...> &) -> packaged_task<Args...> &:
      annotation:
        - deleted
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: const packaged_task<Args...> &
          unnamed: true
      description: __OPTIONAL__
      return: __OPTIONAL__
      signature_with_names: auto operator=(const packaged_task<Args...> &) -> packaged_task<Args...> &
    auto operator=(packaged_task<Args...> &&) -> packaged_task<Args...> &:
      annotation:
        - defaulted
      arguments:
        - description: __OPTIONAL__
          name: x
          type: packaged_task<Args...> &&
      description: __OPTIONAL__
      return: __OPTIONAL__
      signature_with_names: auto operator=(packaged_task<Args...> && x) -> packaged_task<Args...> &
---
