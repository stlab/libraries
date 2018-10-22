---
layout: function
title: stlab::package
tags: [library]
scope: stlab
pure-name: package
brief: Create a packaged_task/future pair
annotation: template function
example: package_example.cpp
defined-in-header: stlab/concurrency/future.hpp
entities:
  - kind: overloads
    name: stlab::package
    list:
      - name: package
        pure-name: package
        declaration: |
            template <typename Sig, typename E, typename F>
            auto package(E executor, F f) ->
                std::pair<detail::packaged_task_from_signature_t<Sig>,
                          future<detail::result_of_t_<Sig>>>
        description: The template function package creates a pair of a packaged_task and a future.
  - kind: parameters
    list:
      - name: executor
        description: The passed function will run on this executor
      - name: f
        description: Callable object to call
  - kind: result
    description: A std::pair of a packaged_task and the associated future.
---
