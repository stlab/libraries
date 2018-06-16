---
title: Performance/WhatToTest
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Performance/WhatToTest
---

What things need to be tested as part of this benchmark suite?

If you have additions or suggestions, please add them.

#### Contents
{:.no_toc}
1. contents
{:toc}

## Fundamental Concepts
Most of these can't easily be tested.
* regular types
* basic math
* basic flow control
* dereference
    * a pointer/iterator is a regular type that can be dereferenced
* type conversion
    * performance of type conversions (known to be a problem on certain compilers)



## Language Concepts
* abstraction
    * what happens to performance when I put { } around a value?

* inheritance
    * what happens if my function is in a child class, virtual, or both?
    * what happens if my values are in a child class?

* function calls - how expensive are different styles of function call?
    * inline, function pointer, defined in another module, etc.

* exceptions - what is the cost of using exceptions?
    * throwing versus not
    * compared to error return codes
    * exceptions enabled or not in the compiler
    * compared to setjmp/longjmp?

* RTTI - what are the costs?
    * compared to per-object ids or strings

* function objects - what are the costs/benefits compared to inline functions and function pointers?

* template instantiation
    * are there any problems with duplicate template bodies and code size?
    * are there any problems with recursive templates or deep usage of templates?

* constructors
    * Are empty constructors correctly optimized away?
    * Are copy constructors correctly optimized away?
    * Are default copy constructors efficient?

## Simple Idioms
These are building blocks, with more than one way to express them.  These are commonly reused things, usually found in headers like "inlines.h", "math_utils.h", etc.

For all of these:   What is the optimal implementation, and does the compiler recognize other implementations and substitute the optimal one?


* swapping 2 values
    * does anything need to be tested for swapping 2 different types, or is that handled by assignment/conversion testing?
* absolute value
* min/max of 2 values
* switch versus if/else trees
* pin values to range (max, min combo)
* rotate bits within a byte/word
* byte order reversal
* round up/down
* round float to int
* thresholding
* pointer alignment tests and comparisons
* fixed point math
* extended precision math (ie: 64 bit math on a 32 bit CPU)

## Complex Idioms
These may require multiple optimizations to perform well, or some pattern/idiom recognition in the compiler.
These are simple algorithms important to many different tasks/fields, but are frequently the inner loops and most performance sensitive code.
<BR>


* safe bool cast from a smart pointer
* hand coded memcpy, memmove, memset, memcmp
    * simple types and user defined types
    * arrays and containers
* convolution
    * 1D, 2D, and more?
* matrix multiplication
    * 2D is common
    * higher dimensions get a bit esoteric
* min/max of a sequence
* summation of a sequence
* product of a sequence
* dot product of sequences
* sorting a sequence
    * many possible algorithms and implementations
    * given the permutations, is this testable?
    * maybe just test STL implementations?
* reversing a sequence
* rotating members of a sequence
* find/search sequence
    * again, many possible algorithms and implementations
    * given the permutations, is this testable?
    * maybe just test STL implementations?
* lookup tables
* interleave/deinterleave buffers
* transpose block
* rotate a 2D block
    * plus or minus 90 degrees
    * 180 degrees
    * horizontal flip
    * vertical flip
    * transpose
* histogram building
* stack like operations
* bitarrays and manipulations of them
* complex type and template

## Runtime and Library Support
These are library functions or header defined functions/macros supplied by the compiler or OS. These can sometimes be the performance limiting factor in a variety of applications.


* allocation / deletion
    * new, delete
    * malloc, free
    * performance for various sizes (testing suballocation)

* mathlib
    * all functions
    * compared to inline versions of some functions

* string manipulation routines
    * strcpy, strcmp, strchr, strcat, etc.
    * strncpy, strncmp, strncat, etc.

* memory routines
    * memcpy, memmove, memset, memcmp, etc.

* ctype routines
    * isdigit, isalpha, etc.

* stdio
    * printf, getc, fread, fwrite, etc.

* C++ iostreams
    * cout, ostream, etc.
    * compare cout versus printf

* stdlib
    * strtod, strtof

* threads
    * creation, destruction, sleep overhead, semaphores, mutexes

## More Complex Stuff
* STL containers
    * insertion
        * push_front
        * push_back
        * in-order, reverse-order and random insert for ordered containers
    * removal
    * deletion of container
    * iterating items
    * reverse iterating items
    * copy containers
    * erase all entries
    * search/find/lookup
        * in-order, reverse-order and random
    * vector
        * is iteration of vector the same as iterating a pointer
        * is operator[] the same speed as indexing a pointer
        * what is the penalty for using operator at() range checking

* STL algorithms
* iostreams


* strstr / text searching
    * no "best" algorithm
    * large variance in optimum based on alphabet (roman text, gene sequencing, bit pattern)

* Discrete Wavelet/Fourier/Cosine/Sine Transform
    * 1D (Audio, PDE spectral methods)
    * 2D (video, image compression)
    * simple and optimized algorithms
    * wavelet and FFT have many near optimal implementations
    * JPEG, MPEG

## Optimizations
We need to test as many optimizations as possible.  Priority should be given to those that are used most often and offer the highest benefit to the most code (which isn't always easy to judge).



* algebraic simplification
* constant folding
* constant propagation
* copy propagation
* common subexpression elimination
* loop invariant code motion
* dead code elimination
* strength reduction
* value range propagation (aka Predicate Simplification?)
* loop fusion
* loop interchange
* loop normalization
* loop removal
* loop unrolling
* loop unswitching
* normalization of references
* scalar replacement of arrays
* scalar replacement of structs
* instruction scheduling
* removal of unused objects/allocations
    * this may not be legal in some circumstances
* inlining
    * keyword inline
    * auto inline of small functions
    * functions defined in classes
    * functions defined in templates
    * depth of inlining (could be difficult to test)
    * leaf first or trunk first?
    * what factors might disable inlining on some compilers?

## Conformance to Specification
* user specified swap in STL algorithms
* short circuit evaluation of boolean expressions
* growth policies and complexities of STL containers
* concept checks compile away to nothing

## Binary Size
* Are duplicate templates correctly collapsed?
* Are unused template member functions correctly removed?
* Are unused (or duplicate) static, const, and static const values correctly removed?



## Language Extensions
* [Boost](https://www.boost.org/)
* [OpenMP](http://www.openmp.org/)



## Other ?
* ineffiency returning or working with std::pair ?
