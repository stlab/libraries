---
layout: method
title: send
owner: fbrereto
brief: Send tasks for the actor to execute
tags:
  - method
defined_in_file: concurrency/actor.hpp
overloads:
  "template <typename F, typename... Args>\nauto send(F &&, Args &&...)":
    arguments:
      - description: __OPTIONAL__
        name: f
        type: F &&
      - description: __OPTIONAL__
        name: args
        type: Args &&...
    description: __OPTIONAL__
    return: __OPTIONAL__
    signature_with_names: "template <typename F, typename... Args>\nauto send(F && f, Args &&... args)"
---
