---
layout: function
title: stlab::join
tags: [library]
scope: stlab
pure-name: join
brief: Creates a future that joins all passed arguments. This function is deprecated and it will soon be removed. Please use instead of `zip_with`
annotation: template function
example: zip_with_example.cpp
defined-in-header: stlab/concurrency/channel.hpp
entities:
  - kind: overloads
    list:
      - name: join
        pure-name: join
        declaration: |
            template <typename E, typename F, typename...R>
            [[deprecated]] auto join(E e, F f, R... upstream_receiver)
        description: This function creates a new receiver and attaches the process `f` to it. The values coming from the upstream receiver are the parameters of `f`. The incoming upstream values are not passed one after the other to this process, but they are passed as a complete set of arguments to the process. So the last incoming upstream value triggers the execution of `f`.
  - kind: parameters
    list:
      - name: e
        description: Executor which shall be used to execute the task of `f`.
      - name: f
        description: Callable object that implements the task. Its parameters correspond to the results from the upstream receivers. It is called when all upstream receiver have provided its values.
      - name: upstream_receiver
        description: The upstream receiver.
  - kind: result
    description: a future that joins all passed arguments
---
