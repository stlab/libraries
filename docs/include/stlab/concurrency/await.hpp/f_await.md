---
layout: function
title: await
hyde:
  owner: sean-parent
  brief: Waits until the future is ready and returns the future value.
  tags:
    - function
  inline:
    brief: _multiple descriptions_
  defined_in_file: stlab/concurrency/await.hpp
  overloads:
    "template <class T>\nauto await(const future<T> &) -> T":
      annotation:
        - deprecated("implicit copy deprecated, use `await(std::move(f))` or `await(stlab::copy(f))` instead.")
      arguments:
        - description: __OPTIONAL__
          name: x
          type: const future<T> &
      description: __INLINED__
      inline:
        description:
          - Equivalent to `await(copy(x))`.
      return: __OPTIONAL__
      signature_with_names: "template <class T>\nauto await(const future<T> & x) -> T"
    "template <class T>\nauto await(future<T> &&) -> T":
      arguments:
        - description: __OPTIONAL__
          name: x
          type: future<T> &&
      description: __INLINED__
      inline:
        description:
          - Synchronously wait for the result `x`. If `x` resolves as an exception, the exception is rethrown. When using the portable task system, an additional thread is added to the pool if no threads are available and the maximum number of threads has not been reached.
      return: __OPTIONAL__
      signature_with_names: "template <class T>\nauto await(future<T> && x) -> T"
  namespace:
    - stlab
    - v3
---
