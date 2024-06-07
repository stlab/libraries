---
layout: directory
title: concurrency
hyde:
  owner: sean-parent
  brief: This library provides high-level abstractions for implementing algorithms that ease using multiple CPU cores while minimizing contention.

  description: |
    The `stlab::future` implementation differs in several aspects from the C++11/14/17 `std::future`:

    - Provides continuations (`then`) and joins (`when_all` and `when_any`).
    - Support splitting by attaching multiple continuations to a single `future.`
    - Automatically cancel any uniquely contributing tasks when a `future` is destroyed.
    - Supports custom executors.
    - Auto-reduces, i.e., a `future<future<T>>` is automatically converted to a `future<T>`.

    The library provides a default executor that uses the system thread pool, if available. Otherwise, it uses a portable, high-performance, elastic, task-stealing executor. The library also provides a number of utilities and experimental features.
---

### Requirements

- C++17 compliant compiler (clang, gcc, Visual Studio)
- boost (for executing the unit tests)

### Authors

Sean Parent, Foster Brereton, Felix Petriconi and other contributors.
