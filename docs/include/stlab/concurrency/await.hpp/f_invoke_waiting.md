---
layout: function
title: invoke_waiting
hyde:
  owner: sean-parent
  brief: When calling a waiting operation, other than `await()` or `await_for()`, such as directly waiting on a `std::condition_variable`, the waiting operation should be invoked with `invoke_waiting()`. This will ensure that there are enough threads available in the portable default executor pool to resolve any pending dependent tasks. With the other default executors, this operation has no effect. Platform executors have similar capability but are hooked into blocking primitives to directly detect blocking calls.
  tags:
    - function
  inline:
    brief:
      - Assumes f _will wait_ and wakes or adds a thread to the thread pool (to the limit) before invoking f.
  defined_in_file: stlab/concurrency/await.hpp
  overloads:
    "template <class F>\nauto invoke_waiting(F &&)":
      arguments:
        - description: __OPTIONAL__
          name: f
          type: F &&
      description: __INLINED__
      inline:
        description:
          - Assumes f _will wait_ and wakes or adds a thread to the thread pool (to the limit) before invoking f.
      return: __OPTIONAL__
      signature_with_names: "template <class F>\nauto invoke_waiting(F && f)"
  namespace:
    - stlab
    - v3
---
