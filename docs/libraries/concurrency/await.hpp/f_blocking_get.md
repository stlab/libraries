---
layout: function
title: blocking_get
owner: sean-parent
brief: _Deprecated_ Use [`await()`](./f_await_for.html).
tags:
  - function
defined_in_file: concurrency/await.hpp
overloads:
  "template <class T>\nauto blocking_get(future<T>, const std::chrono::nanoseconds &) -> decltype(x.get_try())":
    annotation:
      - deprecated (Use await_for instead.)
    arguments:
      - description: The future on which shall be waited until the result is ready
        name: x
        type: future<T>
      - description: __OPTIONAL__
        name: timeout
        type: const std::chrono::nanoseconds &
    description: Waits as long as the future is fulfilled, or throws the occurred exception
    return: The result of the future x
    signature_with_names: "template <class T>\nauto blocking_get(future<T> x, const std::chrono::nanoseconds & timeout) -> decltype(x.get_try())"
  "template <typename T>\nT blocking_get(future<T>)":
    annotation:
      - deprecated (Use await instead.)
    arguments:
      - description: The future on which shall be waited until the result is ready
        name: x
        type: future<T>
    description: Waits as long as the future is fulfilled, or throws the occurred exception
    return: The result of the future x
    signature_with_names: "template <typename T>\nT blocking_get(future<T> x)"
namespace:
  - stlab
  - v1
---

Waits until the passed future is ready. Using this function causes that one thread resource is consumed which increases contention and possibly causing a deadlock. As well any subsequent non-dependent calculations on the task are also blocked. It is strongly recommended to avoid using this function, but use continuations.
