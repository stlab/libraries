---
title: "Execution Library Boundary and Ownership"
entity_type: observation
confidence: 0.80
created: 2026-10-14T21:52:19Z
last_accessed: 2026-10-14T21:52:19Z
last_reinforced: 2026-10-14T21:52:19Z
access_count: 1
sources:
  - "CMakeLists.txt:29-64"
  - "include/stlab/concurrency/executor_base.hpp:32-50"
  - "src/CMakeLists.txt:1-12"
relationships:
  - type: applies_to
    target: "Architecture Overview"
tags: ["stlab-execution", "abi", "executors", "architecture", "boundary"]
tier: knowledge
source: inferred
---

# Execution Library Boundary and Ownership

The execution and scheduling layer—including executors (`default_executor`, `immediate_executor`, `main_executor`), `stlab::task<Sig>`, system timers, and process lifecycle (`pre_exit`)—is owned and maintained in the external `stlab-execution` repository, not inside this source tree.

Key invariants for developers modifying STLab:
1. Header-only consumption: STLab consumes execution primitives under existing include paths like `<stlab/concurrency/executor_base.hpp>`, but their concrete implementations live in the dependency.
2. Linkage: Consuming targets link `stlab::stlab` (which pulls in `stlab::execution`) or `stlab::execution` directly. `stlab::stlab-core` is an INTERFACE compatibility target, not a secondary runtime.
3. ABI Boundary: STLab components must communicate with process-shared execution state across execution's versioned C ABI (e.g. `stlab_v2_*`), never referencing internal C++ runtime symbols.
4. Concurrency headers must not branch directly on `STLAB_TASK_SYSTEM`, `STLAB_CORE_SHARED`, or `_WIN32`; backend resolution belongs entirely to `stlab-execution`.

Aliases / also known as: Execution Library Boundary, stlab-execution, executor separation, execution ABI boundary
