---
layout: method
title: invoke
hyde:
  owner: sean-parent
  brief: __INLINED__
  tags:
    - method
  inline:
    brief:
      - "NOTE (sean-parent): `Args` are _not_ universal references. This is a `concrete` interface for the model. Do not add ` & & `, that would make it an rvalue reference. The `forward <Args >` here is correct. We are forwarding from the client defined signature to the actual captured model."
  defined_in_file: stlab/concurrency/task.hpp
  overloads:
    static auto invoke(void *, Args...) -> R:
      arguments:
        - description: __OPTIONAL__
          name: self
          type: void *
        - description: __OPTIONAL__
          name: args
          type: Args...
      description: __INLINED__
      inline:
        description:
          - "NOTE (sean-parent): `Args` are _not_ universal references. This is a `concrete` interface for the model. Do not add ` & & `, that would make it an rvalue reference. The `forward <Args >` here is correct. We are forwarding from the client defined signature to the actual captured model."
      return: __OPTIONAL__
      signature_with_names: static auto invoke(void * self, Args... args) -> R
---
