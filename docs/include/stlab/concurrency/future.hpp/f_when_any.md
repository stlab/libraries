---
layout: function
title: when_any
hyde:
  owner: sean-parent
  brief: Creates a future that continues on the first success of any futures passed
  tags:
    - function
  inline:
    brief: _multiple descriptions_
  defined_in_file: stlab/concurrency/future.hpp
  overloads:
    "template <class E, class F, class I>\nauto when_any(const E &, F &&, std::pair<I, I>)":
      arguments:
        - description: Executor which is used to schedule the resulting task
          name: executor
          type: const E &
        - description: Callable object that implements the continuing task
          name: f
          type: F &&
        - description: Describes the range of futures. If an empty range is provided then an stlab::future_exception with code stlab::future_errc::broken_promise is thrown.
          name: range
          type: std::pair<I, I>
      description: __INLINED__
      inline:
        description:
          - "***********************************************************************************************"
      return: __OPTIONAL__
      signature_with_names: "template <class E, class F, class I>\nauto when_any(const E & executor, F && f, std::pair<I, I> range)"
    "template <class E, class F, class T, class... Ts>\nauto when_any(E &&, F &&, future<T> &&, future<Ts> &&...)":
      arguments:
        - description: Executor which is used to schedule the resulting task
          name: executor
          type: E &&
        - description: Callable object that implements the continuing task
          name: f
          type: F &&
        - description: __OPTIONAL__
          name: arg
          type: future<T> &&
        - description: __OPTIONAL__
          name: args
          type: future<Ts> &&...
      description: __INLINED__
      inline:
        description:
          - "***********************************************************************************************"
      return: __OPTIONAL__
      signature_with_names: "template <class E, class F, class T, class... Ts>\nauto when_any(E && executor, F && f, future<T> && arg, future<Ts> &&... args)"
  namespace:
    - stlab
    - v2_1_0
---

Creates a future that continues on the first success of any futures passed. The results of all passed futures must be convertible to the first parameter of the continuing function. The function must accept the a second parameter of std::size_t. It will receive the index of the future that succeeded first.
