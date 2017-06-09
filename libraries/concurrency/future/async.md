---
layout: free-function
title: stlab::async
tags: [library]
scope: stlab
pure-name: async
brief: Run a function on a specified executor
annotation: template function
example: async_example.cpp
entities:
  - kind: overloads
    name: stlab::async
    defined-in-header: stlab/concurrency/future.hpp
    git-link: https://github.com/stlab/libraries/blob/develop/stlab/concurrency/future.hpp
    list:
      - name: async
        pure-name: async
        declaration: |
            template <typename E, typename F, typename ...Args>
            future<std::result_of_t<F (Args...)>> async(E executor, F&& f, Args&&... args)
        description: The template function async runs the function f asynchronously on the specified executor and returns a stlab::future that will eventually hold the result of that function call.
  - kind: parameters
    list:
      - name: executor
        description: The passed function will run on this executor
      - name: f
        description: Callable object to call
      - name: args
        description: parameters to pass to f
  - kind: result
    description: stlab::future referring to the shared state created by this call to std::async.
---
