---
layout: method
title: actor<T>
owner: fbrereto
brief: Constructor
tags:
  - method
defined_in_file: concurrency/actor.hpp
is_ctor: true
overloads:
  actor<T>():
    annotation:
      - default
    description: default constructor
    return: __OPTIONAL__
    signature_with_names: actor<T>()
  "template <class Executor, class... Args>\nactor<T>(Executor &&, std::string &&, Args &&...)":
    arguments:
      - description: An executor upon which this actor will run when it has tasks.
        name: e
        type: Executor &&
      - description: Runtime name of the actor. While the actor is running, its thread will be temporarily given this name. This name can be reconfigured with a call to `set_name`.
        name: name
        type: std::string &&
      - description: Initialization arguments for the instance of `T` the actor holds
        name: args
        type: Args &&...
    description: executor-based constructor
    return: __OPTIONAL__
    signature_with_names: "template <class Executor, class... Args>\nactor<T>(Executor && e, std::string && name, Args &&... args)"
---
