---
title: "Architecture Overview"
entity_type: concept
confidence: 0.80
created: 2026-10-14T21:51:56Z
last_accessed: 2026-10-14T21:51:56Z
last_reinforced: 2026-10-14T21:51:56Z
access_count: 1
sources:
  - "include/stlab/concurrency/future.hpp:50-110"
  - "include/stlab/concurrency/channel.hpp:12-35"
  - "CMakeLists.txt:29-64"
tags: ["architecture", "concurrency", "stlab", "stlab-execution", "future", "channel", "forest"]
tier: knowledge
source: inferred
---

# Architecture Overview

STLab is a header-heavy C++ concurrency and data-structure library (namespace `stlab`).
Its public API is divided into two primary layers:
1. Concurrency primitives:
   - `future<T>` / `package()` (`include/stlab/concurrency/future.hpp`): lazy, value-semantic, non-blocking futures supporting `.then()`, `.recover()`, operator pipe (`|`), and C++20 coroutines (`co_await`).
   - Channels (`include/stlab/concurrency/channel.hpp`): CSP-style reactive pipelines with `sender<T>` and `receiver<T>`.
   - Serial queue (`include/stlab/concurrency/serial_queue.hpp`): serial task dispatch built on top of underlying executors.
   - Await helpers (`include/stlab/concurrency/await.hpp`): explicit synchronization points built on executor scheduling.
2. Independent data structures and utilities:
   - `forest<T>` (`include/stlab/forest.hpp`): ordered tree-of-sequences container offering bidirectional fullorder, preorder, postorder, and child iterators.
   - Algorithms, memory utilities, and functional wrappers under `include/stlab/`.

External dependency architecture:
- Execution scheduling, thread naming, system timers, task abstraction (`stlab::task`), and process shutdown (`pre_exit`) were extracted into the external `stlab-execution` library (`stlab::execution`). STLab headers consume these via `<stlab/concurrency/executor_base.hpp>` and related interfaces.
- `src/stlab.cpp` provides the library anchor for shared builds; STLab itself has minimal compiled code.

Aliases / also known as: Architecture Overview, stlab architecture, STLab concurrency architecture, stlab-execution boundary
