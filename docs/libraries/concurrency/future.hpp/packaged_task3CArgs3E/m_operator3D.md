---
layout: method
title: operator=
owner: __MISSING__
brief: __MISSING__
tags:
  - method
defined_in_file: concurrency/future.hpp
overloads:
  packaged_task<Args...> & operator=(const packaged_task<Args...> &):
    arguments:
      - description: __OPTIONAL__
        name: x
        type: const packaged_task<Args...> &
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: packaged_task<Args...> & operator=(const packaged_task<Args...> & x)
  packaged_task<Args...> & operator=(packaged_task<Args...> &&):
    annotation:
      - default
    arguments:
      - description: __OPTIONAL__
        name: x
        type: packaged_task<Args...> &&
    description: __MISSING__
    return: __OPTIONAL__
    signature_with_names: packaged_task<Args...> & operator=(packaged_task<Args...> && x)
---
