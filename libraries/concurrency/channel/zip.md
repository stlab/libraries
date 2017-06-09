---
layout: free-function
title: stlab::zip
tags: [library]
scope: stlab
pure-name: zip
brief: Creates a future that zips all passed arguments
annotation: template function
example: zip_example.cpp
entities:
  - kind: overloads
    defined-in-header: stlab/concurrency/channel.hpp  
    git-link: https://github.com/stlab/libraries/blob/develop/stlab/concurrency/channel.hpp
    list:
      - name: zip
        pure-name: zip
        declaration: |
            template <typename E, typename F, typename...R>
            auto zip(E e, F f, R&&... upstream_receiver)
        description: This function creates a process that executes the provided function object with the results from the upstream process. The results are passed in a round-robin manner, starting with the result from the first receiver.
  - kind: parameters
    list:
      - name: e
        description: Executor which is used to schedule the resulting task
      - name: f
        description: Callable object that implements the process. All results from the upstream process must be convertable to the only argument of the provided function object's function operator or the argument of process' await function.
      - name: upstream_receiver
        description: The upstream receiver(s). 
  - kind: result
    description: a future that zips all passed arguments
---
