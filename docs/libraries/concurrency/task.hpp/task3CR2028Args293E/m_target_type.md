---
layout: method
title: stlab::task::target_type
tags: [library]
scope: task
pure-name: target_type
defined-in-header: stlab/concurrency/task.hpp
brief: Get type information about the underlying function
entities:
  - kind: methods
    list:
      - name: stlab::task::target_type
        pure-name: target_type
        defined-in-header: stlab/concurrency/task.hpp 
        declaration: |
            const std::type_info& target_type() const
        description: Gives type information about the underlying function
  - kind: result
    description: |
        Type information about the underlying function
---
