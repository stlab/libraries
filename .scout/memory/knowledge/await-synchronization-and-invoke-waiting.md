---
title: "Await Synchronization and invoke_waiting"
entity_type: observation
confidence: 0.70
created: 2026-10-14T21:53:09Z
last_accessed: 2026-10-14T21:53:09Z
last_reinforced: 2026-10-14T21:53:09Z
access_count: 1
sources:
  - "include/stlab/concurrency/await.hpp:56-98"
relationships:
  - type: applies_to
    target: "Future and Package Model"
  - type: depends_on
    target: "Execution Library Boundary and Ownership"
tags: ["await", "invoke-waiting", "future", "blocking", "synchronization"]
tier: knowledge
source: inferred
---

# Await Synchronization and invoke_waiting

`stlab::await(f)` provides explicit synchronous blocking on an asynchronous `stlab::future<T>`.

Key mechanisms and caveats:
- `invoke_waiting` hook: Instead of bare condition variable waits, `await` routes thread suspension through `invoke_waiting([&] { condition.wait(...); })`. This allows executors with cooperative scheduling or worker pool awareness to yield or steal work while the thread waits.
- Deprecation of implicit copy: Passing a `const future<T>&` is deprecated; callers should use `await(std::move(f))` or `await(stlab::copy(f))` to make ownership and shared state continuation attachment explicit.
- Timed blocking: `blocking_get_guarded` in `detail` provides bounded waits with deadline handling.

Aliases / also known as: Await Synchronization and invoke_waiting, stlab::await, invoke_waiting, synchronous future wait, future blocking
