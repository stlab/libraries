---
layout: free-function
title: stlab::join
tags: [library]
entities:
  - kind: overloads
    defined-in-header: stlab/channel.hpp
    git-link: https://github.com/stlab/libraries/blob/develop/stlab/channel.hpp
    list:
      - name: join
        pure-name: join
        defined-in-header: stlab/channel.hpp
        declaration: |
            template <typename E, typename F, typename...R>
            auto join(E e, F f, R&&... upstream_receiver)
        description: This function creates a process that executes the provided process and passes the results from the upstream receiver to it. 
  - kind: parameters
    list:
      - name: e
        description: Executor which shall be used to execute the resulting task
      - name: f
        description: Callable object that implements the task. Its parameters correspond to the results from the upstream receivers. It is called when all upstream receiver have provided its values.
      - name: upstream_receiver
        description: The upstream receiver.
  - kind: result
    description: a future that joins all passed arguments
---
