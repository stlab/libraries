---
title: "Forest Container and Fullorder Traversal"
entity_type: concept
confidence: 0.75
created: 2026-10-14T21:52:48Z
last_accessed: 2026-10-14T21:52:48Z
last_reinforced: 2026-10-14T21:52:48Z
access_count: 1
sources:
  - "include/stlab/forest.hpp:12-25"
  - "include/stlab/forest.hpp:830-870"
relationships:
  - type: applies_to
    target: "Architecture Overview"
tags: ["forest", "tree", "container", "fullorder", "iterator"]
tier: knowledge
source: inferred
---

# Forest Container and Fullorder Traversal

`stlab::forest<T>` is an ordered container representing a rooted tree of sequences, where every node's children form an ordered sequence.

Key concepts and iterator models:
- Fullorder iteration: The primary traversal abstraction (`forest<T>::iterator`). Every node is visited twice: once on its leading edge (`forest_edge::leading`) and once on its trailing edge (`forest_edge::trailing`).
- Derived traversals: Preorder (`preorder_iterator`), postorder (`postorder_iterator`), and child-only traversals (`child_iterator`, `child_adaptor`) are views constructed over the underlying fullorder sequence.
- Structural algorithms: `include/stlab/forest_algorithms.hpp` provides algorithms specifically tuned for fullorder hierarchies (e.g. subtree traversal, child filtering, pruning).

Aliases / also known as: Forest Container and Fullorder Traversal, stlab::forest, ordered forest, fullorder iterator, tree of sequences
