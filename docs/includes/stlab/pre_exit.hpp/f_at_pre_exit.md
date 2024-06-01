---
layout: function
title: at_pre_exit
hyde:
  owner: sean-parent
  brief: Register the `pre_exit_handler` to be called on [`pre_exit()`](./f_pre_exit.html).
  tags:
    - function
  inline:
    brief:
      - Register a pre-exit handler. The pre-exit-handler may not throw. With C++17 or later it is required to be `noexcept`.
  defined_in_file: stlab/pre_exit.hpp
  overloads:
    void at_pre_exit(stlab::pre_exit_handler):
      arguments:
        - description: __OPTIONAL__
          name: f
          type: stlab::pre_exit_handler
      description: Register the `pre_exit_handler` to be called on [`pre_exit()`](./f_pre_exit.html). The functions are called in the reverse order that they are registered.
      inline:
        description:
          - Register a pre-exit handler. The pre-exit-handler may not throw. With C++17 or later it is required to be `noexcept`.
      return: __OPTIONAL__
      signature_with_names: void at_pre_exit(stlab::pre_exit_handler f)
  namespace:
    - stlab
    - v1
---
