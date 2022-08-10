---
layout: function
title: join
owner: sean-parent
brief: Creates a future that joins all passed arguments. This function is deprecated and it will soon be removed. Please use instead of `zip_with`
tags:
  - function
defined_in_file: concurrency/channel.hpp
overloads:
  "template <typename S, typename F, typename... R>\nauto join(S, F, R...)":
    annotation:
      - deprecated (Use zip_with)
    arguments:
      - description: Executor which shall be used to execute the task of `f`.
        name: s
        type: S
      - description: Callable object that implements the task. Its parameters correspond to the results from the upstream receivers. It is called when all upstream receiver have provided its values.
        name: f
        type: F
      - description: The upstream receiver.
        name: upstream_receiver
        type: R...
    description: Creates a new receiver and attaches the process `f` to it. The values coming from the upstream receiver are the parameters of `f`. The incoming upstream values are not passed one after the other to this process, but they are passed as a complete set of arguments to the process. So the last incoming upstream value triggers the execution of `f`.
    return: a future that joins all passed arguments
    signature_with_names: "template <typename S, typename F, typename... R>\nauto join(S s, F f, R... upstream_receiver)"
namespace:
  - stlab
  - v1
---
