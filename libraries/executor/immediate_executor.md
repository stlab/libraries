---
layout: class
title: stlab::immediate_executor
tags: [library]
entities:
  - kind: class
    name: stlab::immediate_executor
    pure-name: immediate_executor
    defined-in-header: executor/immediate_executor.hpp
    declaration: struct immediate_executor;
    description: 
    member-types:
      - type: result_type
        definition: void
    member-functions:
      - name: template <typename T> void operator()(F&& f) const;
        description: Executes the given function immediately.
      
---
