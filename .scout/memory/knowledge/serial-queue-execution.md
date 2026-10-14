---
title: "Serial Queue Execution"
entity_type: concept
confidence: 0.75
created: 2026-10-14T21:52:57Z
last_accessed: 2026-10-14T21:52:57Z
last_reinforced: 2026-10-14T21:52:57Z
access_count: 1
sources:
  - "include/stlab/concurrency/serial_queue.hpp:169-200"
relationships:
  - type: applies_to
    target: "Architecture Overview"
  - type: depends_on
    target: "Execution Library Boundary and Ownership"
tags: ["serial-queue", "executor", "concurrency", "serialization", "task-dispatch"]
tier: knowledge
source: inferred
---

# Serial Queue Execution

`stlab::serial_queue_t` wraps an underlying executor and guarantees serial, non-overlapping execution of tasks submitted to it.

Key operational characteristics:
- Serialization on concurrent executors: Can be layered on top of concurrent multi-threaded executors (e.g. `default_executor`) to protect invariants without explicit mutex locks.
- Schedule modes: Supports `schedule_mode::single` (drains one task per schedule invocation) and `schedule_mode::all` (drains all currently queued items on the assigned worker before relinquishing).
- Fire-and-forget vs future: `sq.executor()` returns an invocable accepting `void() noexcept` suitable for unmonitored execution. Calling `sq(f, args...)` schedules `f` using `async` and returns a trackable `future<T>`.
- Invariant preservation: Does not execute tasks inline when backpressure or scheduling queues are full; tasks remain queued until their turn.

Aliases / also known as: Serial Queue Execution, stlab::serial_queue_t, serial dispatch, serial executor, task serialization
