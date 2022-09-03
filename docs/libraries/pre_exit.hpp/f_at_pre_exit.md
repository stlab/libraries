---
layout: function
title: at_pre_exit
owner: sean-parent
brief: Register the `pre_exit_handler` to be called on [`pre_exit()`](./f_pre_exit.html).
tags:
  - function
defined_in_file: pre_exit.hpp
overloads:
  void at_pre_exit(stlab::pre_exit_handler):
    arguments:
      - description: __OPTIONAL__
        name: f
        type: stlab::pre_exit_handler
    description: Register the `pre_exit_handler` to be called on [`pre_exit()`](./f_pre_exit.html). The functions are called in the reverse order that they are registered.
    return: __OPTIONAL__
    signature_with_names: void at_pre_exit(stlab::pre_exit_handler f)
namespace:
  - stlab
  - v1
---
