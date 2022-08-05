---
title: Argument Against GC
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Argument_Against_GC
---
#### Contents
{:.no_toc}
1. contents
{:toc}

## Use Cases for Garbage Collection in C++

* Object oriented programming - reference based objects
* Functional programming - avoiding copies of immutable objects
* Trash collection - preventing bad code from crashing or exhausting memory or address space.

## Analysis of Each Use Case

* Current solutions - reference counting and pooled memory
* Look at what happens with reference counting with move semantics
* For trash collection - sight issues with trash and reference counted systems [I had a good one recently - can't find it at the moment - bug easy to google for.]

## Complexity of Current GC for C++ Design

* Type qualifiers vs. modifiers vs. attribute for gc_strict and gc_relaxed - are none of these. We have no experience with such constructs and how they interact with type system.
* The new operator has some unspecified magic which allows it to communicate with the type system to know about strict/weak declarations. This information is not available to anyone writing a replacement allocator (which is quite common) and makes it impossible to use an existing application GC allocator (such as sharing an allocator with JavaScript or Managed C++) unless the system provides it.
* The proposal breaks conventions for RAII - and we have no experience or guidelines. When should one call delete?
* The gc_required and gc_forbidden keywords bifurcate the language.
* Suppress and collect must be used for critical areas of performance.
* No guidance on how multiple collectors in the same application will interact.

* Note that the vast amount of complexity necessary for GC is apparent in the number of keywords and library routines to support it. This is a strong argument that it is of little use for saving bad application - it is just as likely that GC will be misused (if not more so) then memory. Google "unmanaged pointers managed C++" for many examples. Note that as long as we support the C libraries (and C runtime) such issues will always be part of C++.

## Outline a Proposal to Address Some Issues

* Eliminate gc_required
* gc_strict as a type qualifier or attribute
* Definition of a operator new() in the presence of GC
* Use of gc_strict as part of type to guide and check RAII.

## Conclusions

* Complex solution to problem for which simple solutions are widely used and will be improved by C++0x leaving us little need.
* We have little to no experience with the recommended language features which are to be standardized.
* Fixing bad software complex system will never work.
* Recommend minor language changes to improve future GC support - disallow hiding of pointers (xor list trick) as one example.

* Finally - address the "C++ is bad because it has no GC" argument head-on. C++ doesn't generate garbage and so has no need for GC. Clearly Java, C#, Objective C, etc. generate lots of garbage.
