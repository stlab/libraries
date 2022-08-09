---
layout: method
title: packaged_task<Args...>
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: concurrency/future.hpp
is_ctor: true
overloads:
  packaged_task<Args...>():
    annotation:
      - default
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: packaged_task<Args...>()
  packaged_task<Args...>(const packaged_task<Args...> &):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: const packaged_task<Args...> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: packaged_task<Args...>(const packaged_task<Args...> & x)
  packaged_task<Args...>(packaged_task<Args...> &&):
    annotation:
      - default
    arguments:
      - description: __OPTIONAL__
        name: unnamed-0
        type: packaged_task<Args...> &&
        unnamed: true
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: packaged_task<Args...>(packaged_task<Args...> &&)
---
