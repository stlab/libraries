---
layout: function
title: pre_exit
hyde:
  owner: sean-parent
  brief: Invokes `pre_exit_handler`s registered with [`at_pre_exit`](./f_at_pre_exit.html).
  tags:
    - function
  inline:
    brief:
      - Invoke all registered pre-exit handlers in the reverse order they are registered. It is safe to register additional handlers during this operation. Must be invoked exactly once prior to program exit.
  defined_in_file: stlab/pre_exit.hpp
  overloads:
    void pre_exit():
      description: Invokes `pre_exit_handler`s registered with [`at_pre_exit`](./f_at_pre_exit.html) in the reverse order they where registered.
      inline:
        description:
          - Invoke all registered pre-exit handlers in the reverse order they are registered. It is safe to register additional handlers during this operation. Must be invoked exactly once prior to program exit.
      return: __OPTIONAL__
      signature_with_names: void pre_exit()
  namespace:
    - stlab
    - v1
---
