---
layout: function
title: stlab::zip_with
tags: [library]
scope: stlab
pure-name: zip_with
brief: Creates a channel that zips multiple channels into a single one
annotation: template function
example: zip_with_example.cpp
defined-in-header: stlab/concurrency/channel.hpp
entities:
  - kind: overloads
    list:
      - name: zip_with
        pure-name: zip_with
        declaration: |
            template <typename E, typename F, typename... R>
            auto zip_with(E e, F f, R... upstream_receiver)
        description: This function creates a new receiver. The values coming from the upstream receiver collected and when from each upstream receiver a values is available, then it passes them to the process `f`.
  - kind: parameters
    list:
      - name: e
        description: Executor which shall be used to combine the upstream values.
      - name: f
        description: The attached process that is invoked with a new set of values.
      - name: upstream_receiver
        description: The upstream receiver.
  - kind: result
    description: a `receiver<T>` where `T is the result type of `f` 
---
