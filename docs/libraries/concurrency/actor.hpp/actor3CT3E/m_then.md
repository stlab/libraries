---
layout: method
title: then
owner: fbrereto
brief: Continue tasks for the actor to execute pending the completion of some future
tags:
  - method
defined_in_file: concurrency/actor.hpp
overloads:
  "template <typename R, typename F, typename... Args>\nauto then(stlab::future<R> &&, F &&, Args &&...)":
    arguments:
      - description: The future to contine
        name: future
        type: stlab::future<R> &&
      - description: The task to run. The first argument passed to the routine will be the actor. The second argument passed will be the resolved value of the future (if it is not `void`).
        name: f
        type: F &&
      - description: Additional arguments to pass to `f` when it is run
        name: args
        type: Args &&...
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <typename R, typename F, typename... Args>\nauto then(stlab::future<R> && future, F && f, Args &&... args)"
---
