---
layout: class
title: stlab::default_executor
tags: [library]
entities:
  - kind: class
    name: stlab::default_executor
    pure-name: default_executor
    defined-in-header: executor/default_executor.hpp
    declaration: struct default_executor;
    description: 
    member-types:
      - type: result_type
        definition: void
    member-functions:
      - name: template <typename T> void operator()(F&& f) const;
        description: Executes the given function on the operating systems's thread pool. It uses a custom thread pool if the system does not provide a thread pool.
      
---
