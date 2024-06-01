---
layout: function
title: stlab::zip
tags: [library]
scope: stlab
pure-name: zip
brief: It zips the values in step from all upstream receivers. (The functionality has changed after release 1.2.0!)
annotation: template function
example: zip_example.cpp
defined-in-header: stlab/concurrency/channel.hpp  
entities:
  - kind: overloads
    list:
      - name: zip
        pure-name: zip
        declaration: |
            template <typename E, typename...R>
            auto zip(E e, R... upstream_receiver)
        description: This function creates receiver of type `tuple<T...>` where `T...` are the `result_type`s of the passed `upstream_receiver`. Whenever a complete set of values from each upstream receiver has arrived, it passes the tuple with the values downstream.
  - kind: parameters
    list:
      - name: e
        description: Executor which is used to schedule the resulting task
      - name: upstream_receiver
        description: The upstream receiver(s). 
  - kind: result
    description: a `tuple<T...>` where `T...` are the `result_types` of all `upstream_receiver`.
---
