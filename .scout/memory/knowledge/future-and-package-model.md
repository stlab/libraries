---
title: "Future and Package Model"
entity_type: concept
confidence: 0.80
created: 2026-10-14T21:52:33Z
last_accessed: 2026-10-14T21:52:33Z
last_reinforced: 2026-10-14T21:52:33Z
access_count: 1
sources:
  - "include/stlab/concurrency/future.hpp:50-110"
  - "include/stlab/concurrency/future.hpp:1175-1215"
  - "include/stlab/concurrency/future.hpp:1270-1310"
relationships:
  - type: applies_to
    target: "Architecture Overview"
  - type: depends_on
    target: "Execution Library Boundary and Ownership"
tags: ["future", "package", "concurrency", "continuations", "cancellation"]
tier: knowledge
source: inferred
---

# Future and Package Model

`stlab::future<T>` is a lazy, value-semantic, composable future implementation fundamentally distinct from `std::future`.

Key characteristics:
- Value semantics: Futures can be copied, moved, and passed by value. Continuations attach to shared state nodes.
- Continuations via `.then(executor, f)` / `operator|`: Runs `f` on the provided executor when the future's value is available. If no executor is specified, the default executor is used.
- Error handling via `.recover(executor, f)` / `operator^`: Handles both value and exception branches (`future<T>` passed into `f`).
- Cancellation: If all references to a future chain's downstream receivers are dropped before execution begins, unstarted packaged tasks can be canceled without running.
- Coroutines: When C++20 coroutines are enabled (`STLAB_STD_COROUTINES`), `stlab::future<T>` supports `co_await`.
- Packaging: Created via `stlab::package<Sig>(executor, f)`, returning a paired `packaged_task` and `future`.

Aliases / also known as: Future and Package Model, stlab::future, packaged_task, future continuations, lazy future
