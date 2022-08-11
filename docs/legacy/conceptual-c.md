---
title: Conceptual C
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Conceptual_C
---

#### Contents
{:.no_toc}
1. contents
{:toc}

## Goals
* Small (same scale as C) and basically C like
* Match actual machine model, better (more modern) than C
* "Higher level" features such as inheritance and RTTI come from libraries. As with C++ you shouldn't pay for what you don't use but we should do better than C++.
* Exceptions may be the exception to the above rule.
* Memory model which supports threading and thread primitives.
* Thread primitives support the following models:
  * Pipes (thread safe Queue messaging system)
  * parallel algorithms (parallel reduce, for_each, etc. - see Intel Thread Building Blocks)
  * simple atomic increment/decrement for shared structures (ref counting for immutable objects and copy-on-write)
* Full compile time introspection (runtime introspection as a library).
* Support the C linkage model
* "Semantic Spaces"
* Library versioning (part of semantic spaces?)
* Value semantics
* Some level of concept support
* Indexed types (Not quite sure how to build this)
* Functions (with state) are first class
* Simple EBNF based grammar

## Non-Goals
## Grammar
## Reference
