---
layout: free-function
title: stlab::channel
tags: [library]
entities:
  - kind: overloads
    list:
      - name: channel
        pure-name: channel
        defined-in-header: stlab/channel.hpp
        declaration: |
            template <typename T, typename E>
            std::pair<sender<T>, receiver<T>> channel(E e)
        description: This function create a joining future. When all passed args futures are fullfilled, then the continuation tasks defined with f is scheduled on the executor e.
  - kind: parameters
    list:
      - name: e
        description: Executor which is used to sends the passed values from the sender down to the receiver.
  - kind: result
    description: Returns a pair of sender an receiver of type T that form a channel
---
