---
layout: method
title: packaged_task<Args...>
hyde:
  owner: sean-parent
  brief: __OPTIONAL__
  tags:
    - method
  defined_in_file: stlab/concurrency/future.hpp
  is_ctor: true
  overloads:
    packaged_task<Args...>():
      annotation:
        - defaulted
      description: __OPTIONAL__
      signature_with_names: packaged_task<Args...>()
    packaged_task<Args...>(const packaged_task<Args...> &):
      annotation:
        - deleted
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: const packaged_task<Args...> &
          unnamed: true
      description: __OPTIONAL__
      signature_with_names: packaged_task<Args...>(const packaged_task<Args...> &)
    packaged_task<Args...>(packaged_task<Args...> &&):
      annotation:
        - defaulted
      arguments:
        - description: __OPTIONAL__
          name: unnamed-0
          type: packaged_task<Args...> &&
          unnamed: true
      description: __OPTIONAL__
      signature_with_names: packaged_task<Args...>(packaged_task<Args...> &&)
---
