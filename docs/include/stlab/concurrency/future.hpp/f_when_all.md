---
layout: function
title: when_all
hyde:
  owner: sean-parent
  brief: Creates a joining future
  tags:
    - function
  inline:
    brief: _multiple descriptions_
  defined_in_file: stlab/concurrency/future.hpp
  overloads:
    "template <class E, class F, class I>\nauto when_all(const E &, F, std::pair<I, I>)":
      arguments:
        - description: Executor which is used to schedule the resulting task
          name: executor
          type: const E &
        - description: Callable object that implements the continuing task
          name: f
          type: F
        - description: Describes the range of futures. If an empty range is provided then an stlab::future_exception with code stlab::future_errc::broken_promise is thrown.
          name: range
          type: std::pair<I, I>
      description: Creates a joining future. When all passed `args` futures are fulfilled, then the continuation tasks defined with `f` is scheduled on the executor `executor`.
      inline:
        description:
          - "***********************************************************************************************"
      return: __OPTIONAL__
      signature_with_names: "template <class E, class F, class I>\nauto when_all(const E & executor, F f, std::pair<I, I> range)"
    "template <class E, class F, class... Ts>\nauto when_all(const E &, F, future<Ts>...)":
      arguments:
        - description: Executor which is used to schedule the resulting task
          name: executor
          type: const E &
        - description: Callable object that implements the continuing task
          name: f
          type: F
        - description: __OPTIONAL__
          name: args
          type: future<Ts>...
      description: Creates a joining future out of the given range. In case that it is a range of futures of move-only types, the futures are moved internally out of the range into the function.
      inline:
        description:
          - "***********************************************************************************************"
      return: The continuation on the group of passed futures.
      signature_with_names: "template <class E, class F, class... Ts>\nauto when_all(const E & executor, F f, future<Ts>... args)"
  namespace:
    - stlab
    - v2_1_0
---
