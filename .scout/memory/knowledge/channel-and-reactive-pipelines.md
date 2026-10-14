---
title: "Channel and Reactive Pipelines"
entity_type: concept
confidence: 0.75
created: 2026-10-14T21:52:41Z
last_accessed: 2026-10-14T21:52:41Z
last_reinforced: 2026-10-14T21:52:41Z
access_count: 1
sources:
  - "include/stlab/concurrency/channel.hpp:12-35"
  - "include/stlab/concurrency/channel.hpp:1545-1590"
relationships:
  - type: applies_to
    target: "Architecture Overview"
  - type: depends_on
    target: "Execution Library Boundary and Ownership"
tags: ["channel", "sender", "receiver", "csp", "reactive", "pipeline"]
tier: knowledge
source: inferred
---

# Channel and Reactive Pipelines

STLab channels provide Communicating Sequential Processes (CSP) style reactive pipelines using paired `sender<T>` and `receiver<T>` abstractions, enabling many-to-many, multi-use processing graphs (unlike one-shot futures).

Key mechanics and invariants:
- Channel construction: `channel<T>(executor)` creates connected `sender<T>` and `receiver<T>` endpoints.
- Processes: Receivers attach a process that executes when values arrive. Processes can be simple callables or stateful await-processes defining `await()`, `yield()`, and `state() const` returning `process_state_scheduled`.
- The `set_ready()` contract: All downstream receivers participating in a graph MUST invoke `.set_ready()` on the terminal nodes. Data flow is blocked and buffers will not drain if any terminal receiver remains not-ready.
- Combinators: Channels support split (multiple receivers on one sender), merge (multiple upstreams feeding into one downstream), zip, and zip-with combinators.

Aliases / also known as: Channel and Reactive Pipelines, stlab::channel, stlab::sender, stlab::receiver, CSP channels
