---
layout: function
title: operator()
brief: Call operator of this class
defined-in-header: stlab/concurrency/immediate_executor.hpp 
overloads:
  "template <typename F>\nvoid operator()(F)":
    description: Execute the given task immediately.
    arguments:
      - description: The task that shall now be executed.
        name: f
        type: F
    signature_with_names: "template <typename F>\nvoid operator()(F f) const"
---
Executes the given function on this executor